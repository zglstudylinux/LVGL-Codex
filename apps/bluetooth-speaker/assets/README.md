# 素材生成

`source/album_cover.jpg` 是已确认的原始封面。生成后的 `ui/assets/album_cover.c` 使用 RGB565，以便 PC、Linux 和未来 MCU 均直接编译使用。

在 Windows 上重新生成封面资源：

```powershell
.\scripts\convert-image-to-lvgl.ps1 `
  -Source .\apps\bluetooth-speaker\assets\source\album_cover.jpg `
  -Output .\apps\bluetooth-speaker\ui\assets\album_cover.c `
  -Symbol bluetooth_speaker_album_cover -Size 240
```

字体资源由 `npx lv_font_conv` 从开源 Noto Sans SC 生成，字符集严格限制为当前界面文案。
