#include <libnotify/notification.h>
#include <stdio.h>
#include <libnotify/notify.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "batt_image_bytes.h"

long batteryPercentage(void) {
    char* buf = malloc(4);
    int fd = open("/sys/class/power_supply/BAT0/capacity", O_RDONLY);
    if (fd == -1) {
        perror("yabnt: could not open battery info");
        return -1;
    }
    
    int bytes_read = read(fd, buf, 3);
    if (bytes_read == -1) {
        perror("yabnt: could not read battery info");
        close(fd);
        return -1;
    }
    
    buf[bytes_read] = 0;
    close(fd);
    
    int percent = strtol(buf, NULL, 10);
    free(buf);
    return percent;
}

int main(void) {
    long batt_percent = batteryPercentage();
    if (batt_percent == -1) return 1;
    if (batt_percent > 10) return 0;
    
    int fd = open("/tmp/lowbatt.png", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) perror("yabnt: could not create low battery image");
    if (write(fd, batt_image_bytes, sizeof(batt_image_bytes)) == -1) perror("yabnt: could not write low battery image");
    close(fd);
    chmod("/tmp/lowbatt.png", 0444);
    
    notify_init("yabnt");
    NotifyNotification* n = notify_notification_new("Low Battery", "Your computer will power off soon unless plugged into a power outlet.", "/tmp/lowbatt.png");
    
    if (batt_percent <= 5) notify_notification_set_urgency(n, NOTIFY_URGENCY_CRITICAL);
    else notify_notification_set_urgency(n, NOTIFY_URGENCY_NORMAL);
    
    GError* error = NULL;
    if (!notify_notification_show(n, &error)) {
        fprintf(stderr, "yabnt: could not show notification: %s\n", error->message);
        g_error_free(error);
    }
    
    notify_uninit();
    remove("/tmp/lowbatt.png");
    return 0;
}