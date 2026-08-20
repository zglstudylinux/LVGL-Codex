[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [string]$Source,
    [Parameter(Mandatory)]
    [string]$Output,
    [Parameter(Mandatory)]
    [ValidatePattern("^[A-Za-z_][A-Za-z0-9_]*$")]
    [string]$Symbol,
    [ValidateRange(1, 1024)]
    [int]$Size = 240
)

$ErrorActionPreference = "Stop"
Add-Type -AssemblyName System.Drawing

$sourcePath = (Resolve-Path $Source).Path
$outputPath = [System.IO.Path]::GetFullPath($Output)
$outputDirectory = Split-Path $outputPath -Parent
[System.IO.Directory]::CreateDirectory($outputDirectory) | Out-Null

$sourceImage = [System.Drawing.Image]::FromFile($sourcePath)
$bitmap = New-Object System.Drawing.Bitmap($Size, $Size, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
$graphics = [System.Drawing.Graphics]::FromImage($bitmap)
try {
    $graphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $graphics.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
    $graphics.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
    $cropSize = [Math]::Min($sourceImage.Width, $sourceImage.Height)
    $cropX = [int](($sourceImage.Width - $cropSize) / 2)
    $cropY = [int](($sourceImage.Height - $cropSize) / 2)
    $graphics.DrawImage($sourceImage, (New-Object System.Drawing.Rectangle(0, 0, $Size, $Size)),
        $cropX, $cropY, $cropSize, $cropSize, [System.Drawing.GraphicsUnit]::Pixel)

    $builder = New-Object System.Text.StringBuilder
    [void]$builder.AppendLine('#include "album_cover.h"')
    [void]$builder.AppendLine()
    [void]$builder.AppendLine("static const uint8_t ${Symbol}_map[] = {")
    $lineValues = New-Object System.Collections.Generic.List[string]
    for($y = 0; $y -lt $Size; $y++) {
        for($x = 0; $x -lt $Size; $x++) {
            $pixel = $bitmap.GetPixel($x, $y)
            $rgb565 = (($pixel.R -shr 3) -shl 11) -bor (($pixel.G -shr 2) -shl 5) -bor ($pixel.B -shr 3)
            $lineValues.Add(('0x{0:X2}' -f ($rgb565 -band 0xFF)))
            $lineValues.Add(('0x{0:X2}' -f (($rgb565 -shr 8) -band 0xFF)))
            if($lineValues.Count -eq 16) {
                [void]$builder.AppendLine("    $($lineValues -join ', '),")
                $lineValues.Clear()
            }
        }
    }
    if($lineValues.Count -gt 0) {
        [void]$builder.AppendLine("    $($lineValues -join ', '),")
    }
    [void]$builder.AppendLine('};')
    [void]$builder.AppendLine()
    [void]$builder.AppendLine("const lv_image_dsc_t $Symbol = {")
    [void]$builder.AppendLine('    .header.magic = LV_IMAGE_HEADER_MAGIC,')
    [void]$builder.AppendLine('    .header.cf = LV_COLOR_FORMAT_RGB565,')
    [void]$builder.AppendLine('    .header.flags = 0,')
    [void]$builder.AppendLine("    .header.w = $Size,")
    [void]$builder.AppendLine("    .header.h = $Size,")
    [void]$builder.AppendLine("    .header.stride = $($Size * 2),")
    [void]$builder.AppendLine("    .data_size = sizeof(${Symbol}_map),")
    [void]$builder.AppendLine("    .data = ${Symbol}_map,")
    [void]$builder.AppendLine('};')
    [System.IO.File]::WriteAllText($outputPath, $builder.ToString(), (New-Object System.Text.UTF8Encoding($false)))
}
finally {
    $graphics.Dispose()
    $bitmap.Dispose()
    $sourceImage.Dispose()
}
