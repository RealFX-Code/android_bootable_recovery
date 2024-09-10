#include "recovery_ui/ui.h"

/* Prints information about the device / software that's running. */
void print_device_info(Device* device, RecoveryUI* ui);

/* Prints software information */
void print_software_info(RecoveryUI* ui);

/* Prints credits list. */
void print_credits(RecoveryUI* ui);

/* Clears the console */
void clear_console(RecoveryUI* ui);

/* Reboots Samsung devices to download mode */
void reboot_download(RecoveryUI* ui);

/* Runs a command and returns the output. (If command fails it returns "ERROR".) */
std::string cmd(std::string cmd);