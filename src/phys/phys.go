package phys

import "math"
import "math/rand"

type f2 struct {
    x, y float64
}

func (a *f2) norm() float64 {
    return math.Hypot(a.x, a.y)
}

func (a *f2) add(b *f2) *f2 {
    a.x += b.x
    a.y += b.y
    return a
}

func minus(a *f2, b *f2) f2 {
    return f2{a.x - b.x, a.y - b.y}
}

func (a *f2) mult(s float64) *f2 {
    a.x *= s
    a.y *= s
    return a
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
    p.mass = 1.0
    return p
}

func getDistance(p1, p2 *particle) f2 {
    return minus(&p1.pos, &p2.pos)
}

func getForce(p1, p2 *particle, G, RMIN float64) *f2 {
    r := getDistance(p1, p2)
    r_norm := r.norm()
    if (r_norm < RMIN) {
        return &f2{0.0, 0.0}
    }
    return r.mult(-G / (r_norm*r_norm*r_norm))
}

func (p *particle) bounce() *particle {
    if p.pos.x < 0.0 {
        p.pos.x *= -1;
        p.vel.x *= -1;
    } else if p.pos.x >= 1.0 {
        p.pos.x = 2.0 - p.pos.x;
        p.vel.x *= -1;
    }
    if p.pos.y < 0.0 {
        p.pos.y *= -1;
        p.vel.y *= -1;
    } else if p.pos.y >= 1.0 {
        p.pos.y = 2.0 - p.pos.y;
        p.vel.y *= -1;
    }
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

func (system *NaiveSystem) StepSystem() {
    for i := 0; i < len(system.particles); i++ {
        system.particles[i].accel = f2{0.0, 0.0}
        for j := 0; j < len(system.particles); j++ {
            if (i == j) {
                continue
            }
            system.particles[i].accel.add(getForce(&system.particles[i], &system.particles[j], system.G, system.RMIN))
        }
    }
    for i := 0; i < len(system.particles); i++ {
        system.particles[i].vel.add(system.particles[i].accel.mult(system.DT))
        system.particles[i].pos.add(system.particles[i].vel.mult(system.DT))
        system.particles[i].bounce()
    }
}
