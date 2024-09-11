#include <android-base/properties.h>
#include <android-base/stringprintf.h>

#include "recovery_ui/device.h"
#include "recovery_ui/ui.h"

// Stolen from: bootable/recovery/recovery.cpp:171-179
static bool yes_no(Device* device, const char* question1,
                   const char* question2) {
    std::vector<std::string> headers{question1, question2};
    std::vector<std::string> items{" No", " Yes"};
    size_t chosen_item = device->GetUI()->ShowMenu(
        headers, items, 0, true,
        std::bind(&Device::HandleMenuKey, device, std::placeholders::_1,
                  std::placeholders::_2));
    return (chosen_item == 1);
}

void print_device_info(Device* device, RecoveryUI* ui) {
    // Stolen code, from: bootable/recovery/recovery.cpp:565-567, and
    // bootable/recovery/install/wipe_data.cpp:120-122
    std::function<bool()> _confirm_func = [&device]() {
        return yes_no(device, "View device information?",
                      "NOTE: The text may not fit on screen depending on what menu you're in.");
    };
    const std::function<bool()>& confirm_func =
        ui->IsTextVisible() ? _confirm_func : nullptr;
    // First checks if confirm_func exists at all, e.g. Not nullptr,
    // then if confirm_func() isn't true, e.g. user selected no.'
    // So, if the user selected No, the body of this statement runs.
    if (confirm_func && !confirm_func()) {
        return;
    }
    std::string prodoct_board =
        android::base::GetProperty("ro.product.board", "(Unknown)");
    std::string prodoct_brand =
        android::base::GetProperty("ro.product.brand", "(Unknown)");
    std::string prodoct_abilist =
        android::base::GetProperty("ro.product.cpu.abilist", "(Unknown)");
    std::string prodoct_abilist32 =
        android::base::GetProperty("ro.product.cpu.abilist32", "(Unknown)");
    std::string prodoct_abilist64 =
        android::base::GetProperty("ro.product.cpu.abilist64", "(Unknown)");
    std::string prodoct_device =
        android::base::GetProperty("ro.product.device", "(Unknown)");
    std::string prodoct_manufacturer =
        android::base::GetProperty("ro.product.manufacturer", "(Unknown)");
    std::string prodoct_model =
        android::base::GetProperty("ro.product.model", "(Unknown)");
    std::string prodoct_name =
        android::base::GetProperty("ro.product.name", "(Unknown)");
    std::string product_serial =
        android::base::GetProperty("ro.serialno", "(unknown)");
    // Stolen from: bootable/recovery/updater/build_info.cpp:110-118
    std::string product_fingerprint = android::base::StringPrintf(
        "%s/%s/%s:%s/%s/%s:%s/%s",
        android::base::GetProperty("ro.product.brand", "").c_str(),
        android::base::GetProperty("ro.product.name", "").c_str(),
        android::base::GetProperty("ro.product.device", "").c_str(),
        android::base::GetProperty("ro.build.version.release", "").c_str(),
        android::base::GetProperty("ro.build.id", "").c_str(),
        android::base::GetProperty("ro.build.version.incremental", "").c_str(),
        android::base::GetProperty("ro.build.type", "").c_str(),
        android::base::GetProperty("ro.build.tags", "").c_str());
    // Device
    ui->Print("\n >>> Device:\n");
    ui->Print("  Board: %s\n", prodoct_board.c_str());
    ui->Print("  Brand: %s\n", prodoct_brand.c_str());
    ui->Print("  Abilist: %s\n", prodoct_abilist.c_str());
    ui->Print("  Abilist32: %s\n", prodoct_abilist32.c_str());
    ui->Print("  Abilist64: %s\n", prodoct_abilist64.c_str());
    ui->Print("  Device: %s\n", prodoct_device.c_str());
    ui->Print("  Manufacturer: %s\n", prodoct_manufacturer.c_str());
    ui->Print("  Model: %s\n", prodoct_model.c_str());
    ui->Print("  Name: %s\n", prodoct_name.c_str());
    ui->Print("  Serial: %s\n", product_serial.c_str());
    ui->Print("  Fingerprint: %s\n\n", product_fingerprint.c_str());
    return;
}

void print_software_info(RecoveryUI* ui) {
    // CrDroid
    std::string crdroid_build_version =
        android::base::GetProperty("ro.crdroid.build.version", "(unknown)");
    ui->Print("\n >>> Current recovery:\n");
    ui->Print("  Current CrDroid base version: %s\n",
              crdroid_build_version.c_str());
    ui->Print("  Initial fork: crdroid/android_bootable_recovery@17c941e\n");
    return;
}

void print_credits(RecoveryUI* ui) {
    // Credits
    ui->Print("\n >>> Credits:\n");
    ui->Print(
        "  Device Trees & Kernel Sources: github.com/Exynos9611Development\n");
    ui->Print("  ROM: CrDroid Team\n");
    ui->Print("  Built rom: Leah (@tromsobadet on discord)\n\n");
    return;
}

void clear_console(RecoveryUI* ui) {
    // Couldn't figure out a good way to do this, so 120 newlines works i guess.
    std::string x = "";
    for (int i = 0; i < 120; i++) {
        x += "\n";
    }
    ui->PrintOnScreenOnly(x.c_str());
    return;
}

void reboot_download(RecoveryUI* ui) {
    ui->Print("Rebooting to download...\n");
    int x = system("reboot download");
    if (x != 0) {
        ui->Print("Couldn't reboot to DOWNLOAD mode!\n");
        // Two newlines bc stupid curved screen
        ui->Print("Command failed with status: %u\n\n", x);
    }
    return;
}

std::string cmd(std::string cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[1024];  // 1024b is enough for uname.
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 1024, pipe.get()) != NULL) result += buffer;
    }
    return result;
}