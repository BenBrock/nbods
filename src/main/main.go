package main
import "../phys"
import "fmt"

func main() {
    system := phys.RandomNaiveSystem(1000)
    fmt.Println(system)
}
