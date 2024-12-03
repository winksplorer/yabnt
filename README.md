# yabnt - yet another battery notification thing

a stupidly simple piece of code that uses libnotify to create notifications based on your battery level.

## behaviors

### battery% > 10%

exits immediately, no notification.

### battery% <= 10%

creates low battery warning notification with normal urgency

### battery% <= 5%

creates low battery warning notification with critical urgency

## build & installation

1. clone repo `git clone https://github.com/winksplorer/yabnt`
2. install deps, for debian this is `sudo apt install clang libnotify-dev`
3. build `make`
4. install `sudo make install`

## setting up with cron

you can use yabnt with cron to automatically give you warnings.

this example runs yabnt every minute, with messages to /tmp/yabnt.log.

```
* * * * * DISPLAY=:0 yabnt >/tmp/yabnt.log 2>&1
```
