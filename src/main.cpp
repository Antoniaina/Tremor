#include <iostream>
#include <pipewire/pipewire.h>

void log(std::string type, std::string message) {
  std::cout << "[ " << type << " ] " << message << std::endl;
}

int main(int argc, char *argv[]) {
  pw_init(nullptr, nullptr);
  log("INFO", "Pipewire initialized");

  pw_main_loop *loop = pw_main_loop_new(nullptr);
  if (!loop) {
    log("ERROR", "Failed creating main loop");
    return 1;
  }

  pw_context *context = pw_context_new(pw_main_loop_get_loop(loop), nullptr, 0);
  pw_core *core = pw_context_connect(context, nullptr, 0);
  if (!core) {
    log("ERROR", "Failed connecting to Pipewire core");
    pw_main_loop_destroy(loop);
    return 1;
  }

  log("INFO", "Successfully connected to Pipewire core ");

  pw_main_loop_quit(loop);
  pw_core_disconnect(core);
  pw_context_destroy(context);
  pw_main_loop_destroy(loop);
  pw_deinit();

  log("INFO", "Ended successfully");
}
