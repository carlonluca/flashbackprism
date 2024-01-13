# FlashbackPrism



This is a small client for [PhotoPrism](https://github.com/photoprism/photoprism) written in Qt. Its only purpose is to show the photos taken years ago in the same date as today. This is an interesting way to review the photos from your collection. PhotoPrism seems to lack this feature in a way that is comfortable for me, so I took some time to implement it, waiting for an official implementation.

The application includes a GUI to browse the photos in previous years, open them and share. It also includes code monitoring the PhotoPrism server for changes and sending a notification approximately at 8am if new photos are available. In Linux, the notification only arrives when the app is running. In Android, the application includes a service, so it should work even after the application is closed.

<p align="center">
  <img src="docs/shot.webp" width="30%" />
  <img src="docs/notifications.png" width="30%" />
</p>

## Supported Systems

I wrote the application just for me, so I built it for Linux and Android only. It is written with the Qt framework, so it should be simple and quick to port it to iOS, Mac OS and Windows, but a few platform specific portions may need to be added.

## Installation

I did not upload AppImages or other binaries for Linux, so you'll have to build it to use it. For android, I'll upload an apk for arm64, but you'll probably want to compile it anyway if you need some changes.

* v0.1.0 - [Android arm64](https://github.com/carlonluca/flashbackprism/releases/download/v0.1.0/flashbackprism-0.1.0-arm64.apk)

## Defects

The tool is built around my need and is **not tested**, so I expect many bugs to be present. This is a partial list of things that should be improved:

- [ ] AppImage is missing
- [ ] Replace the Android service with AlarmManager to save battery
- [ ] Create a settings page and let the user tune the parameters
- [ ] Improve the UI a bit
- [x] Improve the orientation detection
- [x] Handle videos
- [ ] Handle heic format (Qt has a patch for this, so probably waiting is sufficient)
- [ ] HTTPS is untested. It may work, but I never tested it in any way.

[...]
