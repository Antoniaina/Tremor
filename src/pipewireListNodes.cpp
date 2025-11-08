#include "pipewire/context.h"
#include "pipewire/core.h"
#include "pipewire/loop.h"
#include "pipewire/node.h"
#include <cstdint>
#include <iostream>
#include <pipewire/pipewire.h>
#include <string>
#include <unordered_map>

struct NodeInfo {
  uint32_t id;
  std::string name;
  std::string media_class;
};

static void registry_event_global(void *data, uint32_t id, uint32_t permission,
                                  const char *type, uint32_t version,
                                  const struct spa_dict *props) {
  if (std::string(type) != PW_TYPE_INTERFACE_Node)
    return;

  NodeInfo info;
  info.id = id;

  const struct spa_dict_item *item;
  spa_dict_for_each(item, props) {
    if (std::string(item->key) == "node.name")
      info.name = item->value;
    else if (std::string(item->key) == "media.class")
      info.media_class = item->value;
  }

  std::cout << "[Node] ID=" << info.id << " | Name=" << info.name
            << " | MediaClass=" << info.media_class << std::endl;
}

static void registry_event_global_remove(void *data, uint32_t id) {
  std::cout << "[-] Node removed: ID=" << id << std::endl;
}

int main() {
  pw_init(nullptr, nullptr);

  pw_main_loop *loop = pw_main_loop_new(nullptr);
  pw_context *context = pw_context_new(pw_main_loop_get_loop(loop), nullptr, 0);
  pw_core *core = pw_context_connect(context, nullptr, 0);

  pw_registry *registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);
  if (!registry) {
    std::cerr << "Failed to get registry" << std::endl;
    pw_core_disconnect(core);
    pw_context_destroy(context);
    pw_main_loop_destroy(loop);
    pw_deinit();
    return 1;
  }

  struct spa_hook registry_listener;
  static const pw_registry_events registry_events = {
      PW_VERSION_REGISTRY, registry_event_global, registry_event_global_remove};

  pw_registry_add_listener(registry, &registry_listener, &registry_events,
                           nullptr);
  std::cout << "Callbacks registered \n";

  pw_main_loop_run(loop);

  pw_core_disconnect(core);
  pw_context_destroy(context);
  pw_main_loop_destroy(loop);
  pw_deinit();
  return 0;
}
