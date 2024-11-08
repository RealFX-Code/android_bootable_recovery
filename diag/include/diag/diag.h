#include "recovery_ui/ui.h"

/* Prints information about the device / software that's running. */
void print_device_info(RecoveryUI* ui);

/* Prints software information */
void print_software_info(RecoveryUI* ui);

/* Prints credits list. */
void print_credits(RecoveryUI* ui);

/* Clears the console */
void clear_console(RecoveryUI* ui);

/* Reboots Samsung devices to download mode */
void reboot_download(RecoveryUI* ui);

/* Shows kernel revision. (uname -r) */
void print_kernel_revision(RecoveryUI* ui);

/* Prints kernel buffer (Essentially dmesg) */
void print_kernel_logs(Device* device, RecoveryUI* ui);

/* Prints kernel cmdline */
void print_kernel_cmdline(Device* device, RecoveryUI* ui);