#include <jni.h>
#include <unistd.h>
#include "Includes/obfuscate.h"
#include "Includes/Macros.h"
#include "dobby.h"
#include "Menu/Menu.hpp"

// Variabel Mod
bool isAtkSpeedMod = false;
float (*old_GetNextAtkTime)(void *instance, int aspd);

// Hook Atk Speed
float hook_GetNextAtkTime(void *instance, int aspd) {
    if (isAtkSpeedMod) {
        return 0.01f; 
    }
    return old_GetNextAtkTime(instance, aspd);
}

// Daftar Fitur
jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    const char *features[] = {
            OBFUSCATE("Toggle_No death"),
            OBFUSCATE("999_Toggle_Insta Attack Speed") 
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    jobjectArray ret = (jobjectArray)env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")), env->NewStringUTF(""));
    for (int i = 0; i < Total_Feature; i++) env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    return (ret);
}

#define targetLibName OBFUSCATE("libil2cpp.so")

// Logika Perubahan
void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {
    switch (featNum) {
        case 0:
            PATCH_SWITCH(targetLibName, "0x1079728", "C0 03 5F D6", boolean);
            break;
        case 999:
            isAtkSpeedMod = boolean;
            break;
        default:
            break;
    }
}

// Thread Utama
void hack_thread() {
    while (!isLibraryLoaded(targetLibName)) {
        sleep(1);
    }
#if defined(__aarch64__)
    HOOK(targetLibName, "0x1FD52FC", hook_GetNextAtkTime, old_GetNextAtkTime);
#endif
}

__attribute__((constructor))
void lib_main() {
    std::thread(hack_thread).detach();
}
