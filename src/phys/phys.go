package phys

import "math/rand"

type f2 struct {
    x, y float64
}

type particle struct {
    pos f2
    vel f2
    accel f2
    mass float64
}

func RandomParticle() particle {
    var p particle
    p.pos = f2{rand.Float64(), rand.Float64()}
    p.vel = f2{rand.Float64() - 0.5, rand.Float64() - 0.5}
    return p
}

type NaiveSystem struct {
    particles []particle

    G float64
    DT float64
    RMIN float64
}

func RandomNaiveSystem(n uint) NaiveSystem {
    var system NaiveSystem
    system.particles = make([]particle, n)

    for i := uint(0); i < n; i++ {
        system.particles[i] = RandomParticle()
    }

    system.G = 1.0e-3
    system.DT = 1.0e-4
    system.RMIN = 1.0e-2

    return system
}
