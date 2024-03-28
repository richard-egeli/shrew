package main

import (
	"fmt"
	"image/png"
	"log"
	"os"
	"path/filepath"
	"strings"
)

func main() {
	file, err := os.Open(os.Args[1])
	if err != nil {
		log.Fatal("Failed to read file:", err)
	}
	defer file.Close()

	base := filepath.Base(os.Args[1])
	ext := filepath.Ext(os.Args[1])
	filename := strings.TrimSuffix(base, ext)

	img, err := png.Decode(file)
	if err != nil {
		log.Fatal("Failed to decode:", err)
	}

	bounds := img.Bounds()

	width := bounds.Size().X
	height := bounds.Size().Y
	var data []uint8

	for y := range height {
		for x := range width {
			r, g, b, a := img.At(x, y).RGBA()
			r8, g8, b8, a8 := uint8(r>>8), uint8(g>>8), uint8(b>>8), uint8(a>>8)
			data = append(data, r8, g8, b8, a8)
		}
	}

	str := make([]string, len(data))
	for i, v := range data {
		str[i] = fmt.Sprint(v)
	}

	var builder strings.Builder

	name := strings.ToUpper(filename)
	dataStr := name + "_DATA"
	widthStr := name + "_WIDTH"
	heightStr := name + "_HEIGHT"
	formatStr := name + "_FORMAT"

	builder.WriteString("#define " + widthStr + " " + fmt.Sprint(width) + "\n")
	builder.WriteString("#define " + heightStr + " " + fmt.Sprint(height) + "\n")
	builder.WriteString("#define " + formatStr + "  7\n\n")
	builder.WriteString("static unsigned char " + dataStr + "[" + fmt.Sprint(len(data)) + "] = {\n")
	builder.WriteString(strings.Join(str, ","))
	builder.WriteString("};\n")

	builder.WriteString("#define " + name + "_IMAGE \\\n")
	builder.WriteString("\t(Image){\\\n")
	builder.WriteString("\t\t.data = " + dataStr + ",\\\n ")
	builder.WriteString("\t\t.width = " + widthStr + ",\\\n")
	builder.WriteString("\t\t.height = " + heightStr + ",\\\n")
	builder.WriteString("\t\t.format = " + formatStr + ",\\\n")
	builder.WriteString("\t\t.mipmaps = 1\\\n")
	builder.WriteString("}\n")

	os.WriteFile(os.Args[2], []byte(builder.String()), 0666)
}
