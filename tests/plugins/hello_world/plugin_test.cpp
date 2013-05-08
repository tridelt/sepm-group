#include "gtest/gtest.h"
#include "Plugin.h"

TEST(PluginTest, CanCreatePlugin) {
  auto plugin = make_plugin();
  ASSERT_TRUE(plugin != NULL);
  plugin->destroy();
}

TEST(PluginTest, HasName) {
  auto plugin = make_plugin();
  ASSERT_STREQ(plugin->name().c_str(), "Hello World Plugin");
  plugin->destroy();
}
