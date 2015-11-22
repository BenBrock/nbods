package main
import "../phys"
import "fmt"

const n int = 100000
const G float64 = 1.0e-3
const DT float64 = 1.0e-4
const RMIN float64 = 1.0e-2

// Exit after 10 minutes of 60 FPS video
const max_timesteps int = 36000

func main() {
    system := phys.RandomNaiveSystem(uint(n), G, DT, RMIN, "test_" + fmt.Sprintf("%d", n))

    system.RenderImage()
    for i := 0; i < max_timesteps; i++ {
        system.StepSystem()
        system.RenderImage()
    }
}
