#pragma once
#include "../core/vec3.h"
#include "../physics/body.h"
#include <vector>
#include <memory>

struct OctreeNode {
    Vec3 center;
    double size;
    Vec3 centerOfMass;
    double totalMass;
    std::vector<BodyPtr> bodies;
    std::unique_ptr<OctreeNode> children[8];
    bool isLeaf;
    
    OctreeNode(Vec3 c, double s) : center(c), size(s), centerOfMass(0,0,0), totalMass(0), isLeaf(true) {}
    
    void insert(BodyPtr body) {
        if(isLeaf && bodies.size() < 1) {
            bodies.push_back(body);
            updateMass();
            return;
        }
        
        if(isLeaf) {
            subdivide();
        }
        
        int octant = getOctant(body->pos);
        children[octant]->insert(body);
        updateMass();
    }
    
    void subdivide() {
        isLeaf = false;
        double halfSize = size / 2.0;
        
        for(int i = 0; i < 8; i++) {
            Vec3 offset(
                (i & 1) ? halfSize : -halfSize,
                (i & 2) ? halfSize : -halfSize,
                (i & 4) ? halfSize : -halfSize
            );
            children[i] = std::make_unique<OctreeNode>(center + offset, halfSize);
        }
        
        for(auto& body : bodies) {
            int octant = getOctant(body->pos);
            children[octant]->insert(body);
        }
        bodies.clear();
    }
    
    int getOctant(Vec3 pos) {
        int octant = 0;
        if(pos.x > center.x) octant |= 1;
        if(pos.y > center.y) octant |= 2;
        if(pos.z > center.z) octant |= 4;
        return octant;
    }
    
    void updateMass() {
        if(isLeaf) {
            totalMass = 0;
            centerOfMass = Vec3(0,0,0);
            for(auto& body : bodies) {
                totalMass += body->mass;
                centerOfMass += body->pos * body->mass;
            }
            if(totalMass > 0) centerOfMass = centerOfMass / totalMass;
        } else {
            totalMass = 0;
            centerOfMass = Vec3(0,0,0);
            for(int i = 0; i < 8; i++) {
                if(children[i]) {
                    totalMass += children[i]->totalMass;
                    centerOfMass += children[i]->centerOfMass * children[i]->totalMass;
                }
            }
            if(totalMass > 0) centerOfMass = centerOfMass / totalMass;
        }
    }
    
    Vec3 computeForce(Vec3 pos, double mass, double theta = 0.5) {
        if(totalMass < 1e-10) return Vec3(0,0,0);
        
        Vec3 r = centerOfMass - pos;
        double dist = r.length();
        
        if(isLeaf || (size / dist) < theta) {
            if(dist < 1e-10) return Vec3(0,0,0);
            return r * (Physics::G * mass * totalMass / (dist * dist * dist));
        }
        
        Vec3 force(0,0,0);
        for(int i = 0; i < 8; i++) {
            if(children[i]) {
                force += children[i]->computeForce(pos, mass, theta);
            }
        }
        return force;
    }
};

class Octree {
    std::unique_ptr<OctreeNode> root;
    
public:
    Octree(Vec3 center, double size) {
        root = std::make_unique<OctreeNode>(center, size);
    }
    
    void build(const std::vector<BodyPtr>& bodies) {
        root = std::make_unique<OctreeNode>(root->center, root->size);
        for(auto& body : bodies) {
            if(!body->destroyed) {
                root->insert(body);
            }
        }
    }
    
    Vec3 computeForce(Vec3 pos, double mass) {
        return root->computeForce(pos, mass);
    }
};
