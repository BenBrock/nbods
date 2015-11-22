package render

import "fmt"
import "os"
import "github.com/ungerik/go-cairo"

type RenderSystem struct {
    surface *cairo.Surface
    dir string
    image_num int
}

func NewRenderSystem(width, height int, dir string) RenderSystem {
    var rs RenderSystem
    rs.surface = cairo.NewSurface(cairo.FORMAT_ARGB32, width, height)
    rs.surface.Scale(float64(width), float64(height))
    rs.dir = dir

    return rs
}

func (rs *RenderSystem) ClearImage() {
    rs.surface.SetSourceRGB(0.0, 0.0, 0.0)
    rs.surface.Paint()
}

func (rs *RenderSystem) AddPoint(x, y, r, b float64) {
    rs.surface.SetSourceRGBA(r, 0.0, b, 1.0)
    rs.surface.Rectangle(x, y, 2e-3, 2e-3)
    rs.surface.Fill()

    rs.surface.SetSourceRGBA(r, 0.0, b, 0.2)
    rs.surface.Rectangle(x-1e-3, y-1e-3, 4e-3, 4e-3)
    rs.surface.Fill()
}

func (rs *RenderSystem) WriteImage() {
    os.MkdirAll(rs.dir, 0777)
    fname := fmt.Sprintf("%s/%05d.png", rs.dir, rs.image_num)
    rs.surface.WriteToPNG(fname)
    rs.image_num++
}
