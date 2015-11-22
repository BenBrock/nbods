package main
import "../phys"

func main() {
    system := phys.RandomNaiveSystem(1000)
    system.StepSystem()
    system.StepSystem()
}
