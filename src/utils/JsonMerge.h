#include <ArduinoJson.h>

void merge(JsonObject dst, JsonVariantConst src)
{
  if (src.is<JsonObjectConst>())
  {
    for (auto kvp : src.as<JsonObjectConst>())
    {
      merge(dst.createNestedObject(kvp.key()), kvp.value());
    }
  }
  else
  {
    dst.set(src);
  }
}