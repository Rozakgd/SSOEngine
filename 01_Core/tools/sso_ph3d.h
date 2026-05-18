// ==============================================================================
// SSOEngine v1.7 - OFFICIAL LICENSE AND ATTRIBUTION FILE //
// ==============================================================================
// // AUTHOR & LEAD DEVELOPER: // Rozak // // CONTACT: //
// Email: spctacularstudio@gmail.com // GitHub: https://github.com/Rozaq125 //
// Website: https://ssoengine.pages.dev // // SUPPORT THE PROJECT: // Donationware
// Link: https://saweria.co/omedonation // //
// ------------------------------------------------------------------------------
// MIT LICENSE //
// ------------------------------------------------------------------------------
// Copyright (c) 2026 SSOFoundation// // Permission
// is hereby granted, free of charge, to any person obtaining a copy // of this
// software and associated documentation files (the "Software"), to deal // in the
// software without restriction, including without limitation the rights // to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell // copies of
// the Software, and to permit persons to whom the Software is // furnished to do
// so, subject to the following conditions: // // The above copyright notice and
// this permission notice shall be included in all // copies or substantial
// portions of the Software. // // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT
// WARRANTY OF ANY KIND, EXPRESS OR // IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, // FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER // LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE // SOFTWARE. // //
// ------------------------------------------------------------------------------
// BUILT WITH & SPECIAL THANKS //
// ------------------------------------------------------------------------------
// RAYLIB: (c) Ramon Santamaria (https://www.raylib.com) // COMPILER: MinGW-w64
// Project (GCC Suite) // CONTRIBUTOR: Nathan (Feedback Provider) // // COMMUNITY
// HALL OF FAME (Reddit Feedback & Support): // u/rileygrham // u/Jimmy-M-420 //
// u/gabriel_aplok // u/InitialComfortable70 // u/ghulmar // //
// ------------------------------------------------------------------------------
// PROJECT DESCRIPTION //
// ------------------------------------------------------------------------------
// SSOEngine is a professional-grade, cross-platform 2D/3D game engine SDK //
// built on top of Raylib //
// high-precision physics, advanced camera systems, and a professional build //
// pipeline for Windows and Android distribution. // // VERSION: 1.7 //
// ==============================================================================

#ifndef SSO_PH3D_H
#define SSO_PH3D_H

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>

namespace SSO {
    namespace Physics3D {
        
        // Physics Constants
        constexpr float GRAVITY_EARTH = 9.81f * 3.0f; // Triple gravity for faster fall
        constexpr float DEFAULT_DAMPING = 0.995f;      // Less damping for more realistic movement
        constexpr float DEFAULT_RESTITUTION = 0.3f;    // Less bouncy for realistic stacking
        constexpr float DEFAULT_FRICTION = 0.95f;       // Less friction for more sliding
        constexpr float MIN_VELOCITY = 0.001f;          // Lower threshold for more movement
        
        // Physics Body Structure
        struct RigidBody {
            Vector3 position;
            Vector3 velocity;
            Vector3 acceleration;
            Vector3 angularVelocity;
            Vector3 size;
            float mass;
            float restitution;
            float friction;
            bool isStatic;
            bool isGrounded;
            bool isActive;
            Color color;
            
            RigidBody() {
                position = (Vector3){0, 0, 0};
                velocity = (Vector3){0, 0, 0};
                acceleration = (Vector3){0, 0, 0};
                angularVelocity = (Vector3){0, 0, 0};
                size = (Vector3){1, 1, 1};
                mass = 1.0f;
                restitution = DEFAULT_RESTITUTION;
                friction = DEFAULT_FRICTION;
                isStatic = false;
                isGrounded = false;
                isActive = true;
                color = WHITE;
            }
        };
        
        // Collision Structure
        struct Collision {
            bool hasCollision;
            Vector3 normal;
            float penetrationDepth;
            Vector3 contactPoint;
            
            Collision() {
                hasCollision = false;
                normal = (Vector3){0, 1, 0};
                penetrationDepth = 0.0f;
                contactPoint = (Vector3){0, 0, 0};
            }
        };
        
        // Physics World
        struct PhysicsWorld {
            Vector3 gravity;
            std::vector<RigidBody> bodies;
            float timeStep;
            int maxSubSteps;
            
            PhysicsWorld() {
                gravity = (Vector3){0, -GRAVITY_EARTH, 0};
                timeStep = 1.0f / 60.0f;
                maxSubSteps = 3;
            }
        };
        
        inline static PhysicsWorld world;
        
        // Vector3 Utility Functions
        inline Vector3 Vector3Zero() { return (Vector3){0, 0, 0}; }
        inline Vector3 Vector3Up() { return (Vector3){0, 1, 0}; }
        inline Vector3 Vector3Down() { return (Vector3){0, -1, 0}; }
        
        inline float Vector3LengthSquared(Vector3 v) {
            return v.x * v.x + v.y * v.y + v.z * v.z;
        }
        
        inline bool Vector3Equals(Vector3 a, Vector3 b, float epsilon = 0.001f) {
            return fabs(a.x - b.x) < epsilon && 
                   fabs(a.y - b.y) < epsilon && 
                   fabs(a.z - b.z) < epsilon;
        }
        
        inline Vector3 Vector3Clamp(Vector3 v, float min, float max) {
            return (Vector3){
                fmaxf(min, fminf(max, v.x)),
                fmaxf(min, fminf(max, v.y)),
                fmaxf(min, fminf(max, v.z))
            };
        }
        
        // Bounding Box Functions
        inline Vector3 GetBoundingBoxMin(RigidBody* body) {
            return (Vector3){
                body->position.x - body->size.x / 2.0f,
                body->position.y - body->size.y / 2.0f,
                body->position.z - body->size.z / 2.0f
            };
        }
        
        inline Vector3 GetBoundingBoxMax(RigidBody* body) {
            return (Vector3){
                body->position.x + body->size.x / 2.0f,
                body->position.y + body->size.y / 2.0f,
                body->position.z + body->size.z / 2.0f
            };
        }
        
        // Collision Detection
        inline Collision CheckAABBCollision(RigidBody* bodyA, RigidBody* bodyB) {
            Collision collision;
            
            Vector3 minA = GetBoundingBoxMin(bodyA);
            Vector3 maxA = GetBoundingBoxMax(bodyA);
            Vector3 minB = GetBoundingBoxMin(bodyB);
            Vector3 maxB = GetBoundingBoxMax(bodyB);
            
            // Check overlap on all axes
            bool overlapX = (maxA.x >= minB.x) && (minA.x <= maxB.x);
            bool overlapY = (maxA.y >= minB.y) && (minA.y <= maxB.y);
            bool overlapZ = (maxA.z >= minB.z) && (minA.z <= maxB.z);
            
            collision.hasCollision = overlapX && overlapY && overlapZ;
            
            if (collision.hasCollision) {
                // Calculate penetration depth and normal
                Vector3 centerA = bodyA->position;
                Vector3 centerB = bodyB->position;
                Vector3 diff = Vector3Subtract(centerB, centerA);
                
                // Find the minimum penetration axis
                float penetrationX = fminf(maxA.x - minB.x, maxB.x - minA.x);
                float penetrationY = fminf(maxA.y - minB.y, maxB.y - minA.y);
                float penetrationZ = fminf(maxA.z - minB.z, maxB.z - minA.z);
                
                collision.penetrationDepth = fminf(penetrationX, fminf(penetrationY, penetrationZ));
                
                // Set normal based on minimum penetration
                if (collision.penetrationDepth == penetrationX) {
                    collision.normal = (Vector3){(float)(diff.x > 0 ? 1 : -1), 0.0f, 0.0f};
                } else if (collision.penetrationDepth == penetrationY) {
                    collision.normal = (Vector3){0.0f, (float)(diff.y > 0 ? 1 : -1), 0.0f};
                } else {
                    collision.normal = (Vector3){0.0f, 0.0f, (float)(diff.z > 0 ? 1 : -1)};
                }
                
                // Calculate contact point (average of overlap region)
                Vector3 overlapMin = (Vector3){
                    fmaxf(minA.x, minB.x),
                    fmaxf(minA.y, minB.y),
                    fmaxf(minA.z, minB.z)
                };
                Vector3 overlapMax = (Vector3){
                    fminf(maxA.x, maxB.x),
                    fminf(maxA.y, maxB.y),
                    fminf(maxA.z, maxB.z)
                };
                
                collision.contactPoint = (Vector3){
                    (overlapMin.x + overlapMax.x) / 2.0f,
                    (overlapMin.y + overlapMax.y) / 2.0f,
                    (overlapMin.z + overlapMax.z) / 2.0f
                };
            }
            
            return collision;
        }
        
        inline Collision CheckGroundCollision(RigidBody* body, float groundY = 0.0f) {
            Collision collision;
            
            float bodyBottom = body->position.y - body->size.y / 2.0f;
            
            if (bodyBottom <= groundY) {
                collision.hasCollision = true;
                collision.normal = Vector3Up();
                collision.penetrationDepth = groundY - bodyBottom;
                collision.contactPoint = (Vector3){body->position.x, groundY, body->position.z};
            }
            
            return collision;
        }
        
        // Collision Response
        inline void ResolveCollision(RigidBody* bodyA, RigidBody* bodyB, Collision collision) {
            if (!collision.hasCollision) return;
            
            // Separate bodies (position correction)
            float totalMass = bodyA->mass + bodyB->mass;
            if (totalMass == 0) totalMass = 1.0f;
            
            float ratioA = bodyB->mass / totalMass;
            float ratioB = bodyA->mass / totalMass;
            
            Vector3 separation = Vector3Scale(collision.normal, collision.penetrationDepth * 1.02f); // More overcorrection
            
            if (!bodyA->isStatic) {
                bodyA->position = Vector3Subtract(bodyA->position, Vector3Scale(separation, ratioA));
            }
            if (!bodyB->isStatic) {
                bodyB->position = Vector3Add(bodyB->position, Vector3Scale(separation, ratioB));
            }
            
            // Calculate relative velocity
            Vector3 relativeVelocity = Vector3Subtract(bodyB->velocity, bodyA->velocity);
            float velocityAlongNormal = Vector3DotProduct(relativeVelocity, collision.normal);
            
            // Don't resolve if velocities are separating
            if (velocityAlongNormal > 0) return;
            
            // Calculate restitution (average of both bodies)
            float e = (bodyA->restitution + bodyB->restitution) / 2.0f;
            
            // Calculate impulse scalar with mass consideration
            float j = -(1 + e) * velocityAlongNormal;
            j /= 1/bodyA->mass + 1/bodyB->mass;
            
            // Apply impulse
            Vector3 impulse = Vector3Scale(collision.normal, j);
            
            if (!bodyA->isStatic) {
                bodyA->velocity = Vector3Subtract(bodyA->velocity, Vector3Scale(impulse, 1/bodyA->mass));
                // Apply minimal friction to maintain momentum
                bodyA->velocity.x *= 0.97f;
                bodyA->velocity.z *= 0.97f;
                
                // Add moderate angular velocity for smooth tumbling effect
                Vector3 impactPoint = Vector3Subtract(collision.contactPoint, bodyA->position);
                Vector3 torque = Vector3CrossProduct(impactPoint, impulse);
                bodyA->angularVelocity = Vector3Add(bodyA->angularVelocity, 
                                                   Vector3Scale(torque, 0.5f / bodyA->mass)); // Reduced from 1.0f
                
                // Add moderate random tumbling for realistic effect
                bodyA->angularVelocity = Vector3Add(bodyA->angularVelocity, 
                                                   (Vector3){GetRandomValue(-100, 100) * 0.01f, // Reduced from 0.02f
                                                            GetRandomValue(-100, 100) * 0.01f, 
                                                            GetRandomValue(-100, 100) * 0.01f});
            }
            if (!bodyB->isStatic) {
                bodyB->velocity = Vector3Add(bodyB->velocity, Vector3Scale(impulse, 1/bodyB->mass));
                // Apply minimal friction to maintain momentum
                bodyB->velocity.x *= 0.97f;
                bodyB->velocity.z *= 0.97f;
                
                // Add moderate angular velocity for smooth tumbling effect
                Vector3 impactPoint = Vector3Subtract(collision.contactPoint, bodyB->position);
                Vector3 torque = Vector3CrossProduct(impactPoint, impulse);
                bodyB->angularVelocity = Vector3Add(bodyB->angularVelocity, 
                                                   Vector3Scale(torque, 0.5f / bodyB->mass)); // Reduced from 1.0f
                
                // Add moderate random tumbling for realistic effect
                bodyB->angularVelocity = Vector3Add(bodyB->angularVelocity, 
                                                   (Vector3){GetRandomValue(-100, 100) * 0.01f, // Reduced from 0.02f
                                                            GetRandomValue(-100, 100) * 0.01f, 
                                                            GetRandomValue(-100, 100) * 0.01f});
            }
        }
        
        inline void ResolveGroundCollision(RigidBody* body, Collision collision) {
            if (!collision.hasCollision) return;
            
            // Position correction
            body->position.y += collision.penetrationDepth;
            
            // Velocity response
            float velocityAlongNormal = Vector3DotProduct(body->velocity, collision.normal);
            
            if (velocityAlongNormal < 0) {
                // Apply restitution (less bouncy)
                body->velocity.y *= -body->restitution;
                
                // Apply minimal friction to allow sliding
                body->velocity.x *= 0.95f;
                body->velocity.z *= 0.95f;
                
                // Mark as grounded
                body->isGrounded = true;
                
                // Only stop tiny bounces, allow continued movement
                if (fabs(body->velocity.y) < 0.5f) {
                    body->velocity.y = 0;
                }
                
                // Add realistic tumbling based on horizontal velocity
                float horizontalSpeed = sqrt(body->velocity.x * body->velocity.x + body->velocity.z * body->velocity.z);
                if (horizontalSpeed > 1.0f) {
                    body->angularVelocity = Vector3Add(body->angularVelocity, 
                                                       (Vector3){-body->velocity.z * 0.1f, 
                                                                0, 
                                                                body->velocity.x * 0.1f});
                }
            }
        }
        
        // Force Application
        inline void ApplyForce(RigidBody* body, Vector3 force) {
            if (body->isStatic) return;
            body->acceleration = Vector3Add(body->acceleration, Vector3Scale(force, 1.0f / body->mass));
        }
        
        inline void ApplyImpulse(RigidBody* body, Vector3 impulse) {
            if (body->isStatic) return;
            body->velocity = Vector3Add(body->velocity, Vector3Scale(impulse, 1.0f / body->mass));
        }
        
        inline void ApplyGravity(RigidBody* body) {
            if (body->isStatic || body->isGrounded) return;
            ApplyForce(body, Vector3Scale(world.gravity, body->mass));
        }
        
        // Integration
        inline void Integrate(RigidBody* body, float dt) {
            if (body->isStatic) return;
            
            // Update velocity
            body->velocity = Vector3Add(body->velocity, Vector3Scale(body->acceleration, dt));
            
            // Apply very minimal damping
            body->velocity = Vector3Scale(body->velocity, DEFAULT_DAMPING);
            
            // Apply angular velocity damping (minimal to keep tumbling smooth)
            body->angularVelocity = Vector3Scale(body->angularVelocity, 0.999f);
            
            // Update position
            body->position = Vector3Add(body->position, Vector3Scale(body->velocity, dt));
            
            // Add smooth tumbling effect - if body is falling and not grounded, add moderate angular velocity
            if (!body->isGrounded && body->velocity.y < -2.0f) {
                float randomFactor = 0.8f; // Reduced from 2.0f for smoother movement
                body->angularVelocity = Vector3Add(body->angularVelocity, 
                                                   (Vector3){GetRandomValue(-100, 100) * randomFactor * 0.001f, 
                                                            GetRandomValue(-100, 100) * randomFactor * 0.001f, 
                                                            GetRandomValue(-100, 100) * randomFactor * 0.001f});
            }
            
            // Reset acceleration
            body->acceleration = Vector3Zero();
            
            // Only clamp extremely small velocities to allow natural movement
            if (Vector3LengthSquared(body->velocity) < MIN_VELOCITY * MIN_VELOCITY) {
                body->velocity = Vector3Zero();
            }
            
            // Clamp angular velocity to prevent infinite spinning but allow visible tumbling
            float angularSpeed = Vector3Length(body->angularVelocity);
            if (angularSpeed > 8.0f) { // Reduced from 15.0f
                body->angularVelocity = Vector3Scale(Vector3Normalize(body->angularVelocity), 8.0f);
            }
            
            // Add very small random angular velocity when grounded but still moving (for realistic settling)
            if (body->isGrounded && Vector3Length(body->velocity) > 0.5f) {
                body->angularVelocity = Vector3Add(body->angularVelocity, 
                                                   (Vector3){GetRandomValue(-50, 50) * 0.0005f, // Reduced from 0.001f
                                                            GetRandomValue(-50, 50) * 0.0005f, 
                                                            GetRandomValue(-50, 50) * 0.0005f});
            }
        }
        
        // World Management
        inline void SetGravity(Vector3 gravity) {
            world.gravity = gravity;
        }
        
        inline void SetGravity(float y) {
            world.gravity = (Vector3){0, y, 0};
        }
        
        inline RigidBody* CreateBody(Vector3 position, Vector3 size, Color color, float mass = 1.0f) {
            world.bodies.emplace_back();
            RigidBody* body = &world.bodies.back();
            
            body->position = position;
            body->size = size;
            body->mass = mass;
            
            return body;
        }
        
        inline RigidBody* CreateStaticBody(Vector3 position, Vector3 size, Color color) {
            RigidBody* body = CreateBody(position, size, color, 0.0f);
            body->isStatic = true;
            return body;
        }
        
        inline void RemoveBody(int index) {
            if (index >= 0 && index < world.bodies.size()) {
                world.bodies.erase(world.bodies.begin() + index);
            }
        }
        
        inline void ClearBodies() {
            world.bodies.clear();
        }
        
        inline int GetBodyCount() {
            return world.bodies.size();
        }
        
        inline RigidBody* GetBody(int index) {
            if (index >= 0 && index < world.bodies.size()) {
                return &world.bodies[index];
            }
            return nullptr;
        }
        
        // Simulation Step
        inline void Step(float deltaTime) {
            float time = deltaTime;
            float dt = world.timeStep;
            
            int steps = 0;
            while (time > 0.0f && steps < world.maxSubSteps) {
                float stepTime = fminf(time, dt);
                
                // Reset grounded state
                for (auto& body : world.bodies) {
                    body.isGrounded = false;
                }
                
                // Apply forces
                for (auto& body : world.bodies) {
                    ApplyGravity(&body);
                }
                
                // Integrate
                for (auto& body : world.bodies) {
                    Integrate(&body, stepTime);
                }
                
                // Collision detection and response
                for (size_t i = 0; i < world.bodies.size(); i++) {
                    // Ground collisions
                    Collision groundCollision = CheckGroundCollision(&world.bodies[i]);
                    if (groundCollision.hasCollision) {
                        ResolveGroundCollision(&world.bodies[i], groundCollision);
                    }
                    
                    // Body-body collisions
                    for (size_t j = i + 1; j < world.bodies.size(); j++) {
                        Collision collision = CheckAABBCollision(&world.bodies[i], &world.bodies[j]);
                        if (collision.hasCollision) {
                            ResolveCollision(&world.bodies[i], &world.bodies[j], collision);
                        }
                    }
                }
                
                time -= stepTime;
                steps++;
            }
        }
        
        // Rendering Helper
        inline void DrawBody(RigidBody* body) {
            if (!body->isActive) return;
            
            // Draw the box at its current position
            Vector3 drawPos = body->position;
            
            // Add smooth visual tumbling effect based on angular velocity
            if (Vector3Length(body->angularVelocity) > 0.05f) {
                // Create smooth rotation for tumbling effect
                float time = (float)GetTime();
                float rotX = body->angularVelocity.x * time * 0.5f;
                float rotY = body->angularVelocity.y * time * 0.5f;
                float rotZ = body->angularVelocity.z * time * 0.5f;
                
                // Apply smooth rotation offsets for visual effect
                float offsetX = sin(rotY) * body->size.x * 0.05f;  // Reduced from 0.1f
                float offsetZ = sin(rotX) * body->size.z * 0.05f;  // Reduced from 0.1f
                float offsetY = cos(rotZ) * body->size.y * 0.02f;  // Reduced from 0.05f
                
                drawPos.x += offsetX;
                drawPos.y += offsetY;
                drawPos.z += offsetZ;
            }
            
            // Draw the box
            DrawCube(drawPos, body->size.x, body->size.x, body->size.x, body->color);
            
            // Draw wireframe for better visibility
            DrawCubeWires(drawPos, body->size.x, body->size.x, body->size.x, BLACK);
        }
        
        inline void DrawAllBodies() {
            for (auto& body : world.bodies) {
                DrawBody(&body);
            }
        }
        
        // Utility Functions
        inline void ShootBullet(Vector3 position, Vector3 direction, float force = 50.0f) {
            // Raycast for bullet collision
            Vector3 rayEnd = Vector3Add(position, Vector3Scale(direction, 20.0f));
            
            for (auto& body : world.bodies) {
                if (!body.isStatic) {
                    Vector3 minBounds = GetBoundingBoxMin(&body);
                    Vector3 maxBounds = GetBoundingBoxMax(&body);
                    
                    // Simple ray-AABB intersection test
                    Vector3 invDir = (Vector3){1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z};
                    
                    float t1 = (minBounds.x - position.x) * invDir.x;
                    float t2 = (maxBounds.x - position.x) * invDir.x;
                    float t3 = (minBounds.y - position.y) * invDir.y;
                    float t4 = (maxBounds.y - position.y) * invDir.y;
                    float t5 = (minBounds.z - position.z) * invDir.z;
                    float t6 = (maxBounds.z - position.z) * invDir.z;
                    
                    float tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));
                    float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));
                    
                    if (tmax >= 0 && tmin <= tmax && tmin >= 0 && tmin <= 20.0f) {
                        Vector3 hitPoint = Vector3Add(position, Vector3Scale(direction, tmin));
                        Vector3 toBody = Vector3Subtract(body.position, position);
                        float distance = Vector3Length(toBody);
                        
                        if (distance < 15.0f && distance > 0.1f) {
                            toBody = Vector3Normalize(toBody);
                            float impactForce = force * (1.0f - distance / 15.0f); // Falloff
                            ApplyImpulse(&body, Vector3Scale(toBody, impactForce));
                            
                            // Add some random angular velocity for impact effect
                            body.angularVelocity = Vector3Add(body.angularVelocity, 
                                                               (Vector3){GetRandomValue(-50, 50) * 0.02f, 
                                                                        GetRandomValue(-50, 50) * 0.02f, 
                                                                        GetRandomValue(-50, 50) * 0.02f});
                        }
                    }
                }
            }
        }
        
        inline void Explode(Vector3 center, float force = 100.0f, float radius = 10.0f) {
            for (auto& body : world.bodies) {
                if (!body.isStatic) {
                    Vector3 toBody = Vector3Subtract(body.position, center);
                    float distance = Vector3Length(toBody);
                    
                    if (distance < radius && distance > 0.1f) {
                        toBody = Vector3Normalize(toBody);
                        float impactForce = force * (1.0f - distance / radius); // Falloff
                        ApplyImpulse(&body, Vector3Scale(toBody, impactForce));
                        
                        // Add some random angular velocity for explosion effect
                        body.angularVelocity = Vector3Add(body.angularVelocity, 
                                                           (Vector3){GetRandomValue(-100, 100) * 0.05f, 
                                                                    GetRandomValue(-100, 100) * 0.05f, 
                                                                    GetRandomValue(-100, 100) * 0.05f});
                    }
                }
            }
        }
        
        // Initialization
        inline void Init() {
            world = PhysicsWorld();
        }
        
        inline void Cleanup() {
            ClearBodies();
        }
    }
}

#endif // SSO_PH3D_H
