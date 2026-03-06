#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>

struct Palette {
  const char* name;
  const char* chars;
};

int main(int argc, char** argv) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "Failed to open camera\n";
    return 1;
  }

  cv::Mat frame;
  cv::Mat blurred;
  cv::Mat gray_frame;
  cv::Mat resized_frame;

  const std::vector<Palette> palettes = {
      {"classic", " .:-=+*#%@"},
      {"classic-8", " .:-=+*#%8@"},
      {"soft", " .'`^,:;=*#@"},
      {"balanced", "   .,:;+*?%S#@"},
      {"sharp", "  .,_~:;!>+*=#@"},
      {"dense", " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczMW&8%B@$"}};

  int palette_idx = 3;
  if (argc > 1) {
    palette_idx = std::atoi(argv[1]);
  }
  palette_idx =
      std::max(0, std::min(palette_idx, static_cast<int>(palettes.size()) - 1));
  const std::string chars = palettes[palette_idx].chars;

  std::vector<char> lut(256);
  for (int i = 0; i < 256; ++i) {
    lut[i] = chars[(i * chars.size()) / 256];
  }

  constexpr int target_fps = 30;
  const auto frame_dt = std::chrono::milliseconds(1000 / target_fps);

  std::string ascii_frame;
  std::cout << "\x1b[2J\x1b[H\x1b[?25l";
  while (true) {
    auto t0 = std::chrono::steady_clock::now();

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    cap >> frame;
    cv::flip(frame, frame, 1);

    if (frame.empty()) {
      break;
    }

    cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray_frame, blurred, cv::Size(0, 0), 2.0);
    cv::addWeighted(gray_frame, 1.5, blurred, -0.5, 0, gray_frame);

    int render_width = w.ws_col;
    constexpr int char_aspect = 2;
    int render_height =
        (render_width * frame.rows) / (frame.cols * char_aspect);
    render_height =
        std::max(1, std::min(render_height, static_cast<int>(w.ws_row)));

    cv::resize(gray_frame, resized_frame, cv::Size(render_width, render_height),
               0, 0, cv::INTER_NEAREST);

    size_t total_size = render_height * (render_width + 1);
    if (ascii_frame.size() != total_size) {
      ascii_frame.resize(total_size);
    }

    size_t str_idx = 0;
    for (size_t y = 0; y < render_height; ++y) {
      const uchar* row_ptr = resized_frame.ptr<uchar>(y);
      for (size_t x = 0; x < render_width; ++x) {
        ascii_frame[str_idx++] = lut[row_ptr[x]];
      }
      ascii_frame[str_idx++] = '\n';
    }
    std::cout << "\x1b[H" << ascii_frame;

    auto t1 = std::chrono::steady_clock::now();
    auto spent = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    if (spent < frame_dt) {
      std::this_thread::sleep_for(frame_dt - spent);
    }
  }

  cap.release();
  cv::destroyAllWindows();

  std::cout << "\x1b[?25h";
  return 0;
}
