#include <jni.h>
#include "NitroSteggitOnLoad.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
  return margelo::nitro::steggit::initialize(vm);
}
