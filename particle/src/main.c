#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    float x;
    float y;
} Vector2D;
 
typedef struct {
    int id;
    float mass;
    float radius;
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
} Particle;

typedef struct {
    Particle* p;
    bool isRunning;
    int number_of_particles;
    int clk;
} ParticleSystem;

int rrand(int start, int end){
    return start+rand()%(end-start-1);
}

void particle_info_dump(Particle *p, int id){
    printf("particle: %d mass: %f radius: %f x: %f y: %f vf: %f vy: %f ax: %f ay: %f\n", 
                p[id].id, p[id].mass, p[id].radius, 
                p[id].position.x, p[id].position.y, 
                p[id].velocity.x, p[id].velocity.y,
                p[id].acceleration.x, p[id].acceleration.y);
}

// Randomize particle positions within 0 <= x <= 100, 0 <= y <= 100
void scatter_particles(ParticleSystem W){
    for (int i = 0; i<W.number_of_particles; i++){
        int x = rrand(0,100);
        int y = rrand(0,100);
        W.p[i].position.x = x;
        W.p[i].position.y = y;
    }
}

void set_position(Particle p, float x, float y){
    p.position.x = x;
    p.position.y = y;
}

float distance(Particle p1, Particle p2){
    return sqrtf(powf(p2.position.x-p1.position.x,2) + powf(p2.position.y-p1.position.y,2));
}

int main(int argc, char** argv){
    srand(time(NULL));

    ParticleSystem W;
    W.isRunning = true;

    // general "config" {
    float default_particle_radius = 1;
    float default_particle_mass = 1;

    W.number_of_particles = 2;
    // }
    
    W.clk = 0;

    if (W.number_of_particles < 1){
        printf("no particles? :(");
        return 1;
    }

    Particle *particles = (Particle *)calloc(W.number_of_particles, sizeof(Particle));
    for (int i = 0; i<W.number_of_particles; i++){
        particles[i].id = i;
        particles[i].radius = default_particle_radius;
    }

    W.p = particles;

    // initial particle scatter
    //scatter_particles(W);

    while (W.isRunning) {
        sleep(1);
        W.clk += 1;
        printf("\033cnumber_of_particles: %d\n", W.number_of_particles);

        set_position(W.p[0], 1, 0);
        set_position(W.p[1], 2, 0);

        // Each particle loop
        for (int i = 0; i<W.number_of_particles; i++){
            // Collision checking
            for (int j = 0; j<W.number_of_particles; j++){
                if ((i != j) && (distance(W.p[i],W.p[j]) < default_particle_radius+1)){
                    printf("\nParticle %d collided with particle %d\n",W.p[i].id,W.p[j].id);
                }
            }
            particle_info_dump(W.p, i);
        }
    }

    free(particles);
    return 0;
}