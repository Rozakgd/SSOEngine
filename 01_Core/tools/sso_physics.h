#ifndef SSO_PHYSICS_H
#define SSO_PHYSICS_H

#include "../include/raylib.h"
#include "../include/raymath.h"
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cmath>

namespace SSO {
    namespace Physics {
        
        // --- PHYSICS CONSTANTS ---
        constexpr float GRAVITY = 980.0f;        // pixels/second^2
        constexpr float AIR_RESISTANCE = 0.99f;    // velocity dampening
        constexpr float BOUNCE_DAMPING = 0.7f;      // energy loss on bounce
        constexpr float FRICTION = 0.85f;          // ground friction
        constexpr float GROUND_NORMAL_THRESHOLD = -0.5f; // surface slope threshold for grounded state
        
        // --- PHYSICS OBJECTS ---
        
        struct RigidBody {
            Vector2 position;
            Vector2 velocity;
            Vector2 acceleration;
            float mass;
            float radius;
            bool isGrounded;
            bool isStatic;
            
            RigidBody() {
                position = {0, 0};
                velocity = {0, 0};
                acceleration = {0, 0};
                mass = 1.0f;
                radius = 20.0f;
                isGrounded = false;
                isStatic = false;
            }
            
            RigidBody(Vector2 pos, float r, float m = 1.0f, bool staticObj = false) {
                position = pos;
                velocity = {0, 0};
                acceleration = {0, 0};
                mass = m;
                radius = r;
                isGrounded = false;
                isStatic = staticObj;
            }
        };
        
        struct CollisionInfo {
            bool hasCollision;
            Vector2 normal;
            float penetration;
            RigidBody* bodyA;
            RigidBody* bodyB;
            
            CollisionInfo() {
                hasCollision = false;
                normal = {0, 0};
                penetration = 0.0f;
                bodyA = nullptr;
                bodyB = nullptr;
            }
        };
        
        // --- CORE PHYSICS FUNCTIONS ---
        
        /**
         * Apply force to rigid body (F = ma)
         */
        inline void ApplyForce(RigidBody& body, Vector2 force) {
            if (!body.isStatic) {
                body.acceleration = Vector2Add(body.acceleration, 
                    Vector2Scale(force, 1.0f / body.mass));
            }
        }
        
        /**
         * Apply impulse to rigid body (instant velocity change)
         */
        inline void ApplyImpulse(RigidBody& body, Vector2 impulse) {
            if (!body.isStatic) {
                body.velocity = Vector2Add(body.velocity, 
                    Vector2Scale(impulse, 1.0f / body.mass));
            }
        }
        
        /**
         * Update rigid body with physics simulation
         */
        inline void UpdateRigidBody(RigidBody& body, float deltaTime) {
            if (body.isStatic) return;
            
            // Apply gravity
            body.acceleration.y += GRAVITY;
            
            // Update velocity with acceleration
            body.velocity = Vector2Add(body.velocity, 
                Vector2Scale(body.acceleration, deltaTime));
            
            // Apply air resistance
            body.velocity = Vector2Scale(body.velocity, AIR_RESISTANCE);
            
            // Update position with velocity
            body.position = Vector2Add(body.position, 
                Vector2Scale(body.velocity, deltaTime));
            
            // Reset acceleration for next frame
            body.acceleration = {0, 0};
            
            // Reset grounded state
            body.isGrounded = false;
        }
        
        // --- COLLISION DETECTION ---
        
        /**
         * Circle vs Circle collision detection
         */
        inline bool CheckCircleCollision(const RigidBody& a, const RigidBody& b) {
            float distance = Vector2Distance(a.position, b.position);
            return distance < (a.radius + b.radius);
        }
        
        /**
         * Circle vs Rectangle collision detection
         */
        inline bool CheckCircleRectCollision(const RigidBody& circle, Rectangle rect) {
            Vector2 closestPoint = {
                fmaxf(rect.x, fminf(circle.position.x, rect.x + rect.width)),
                fmaxf(rect.y, fminf(circle.position.y, rect.y + rect.height))
            };
            
            float distance = Vector2Distance(circle.position, closestPoint);
            return distance < circle.radius;
        }
        
        /**
         * Rectangle vs Rectangle collision detection
         */
        inline bool CheckRectCollision(Rectangle a, Rectangle b) {
            return (a.x < b.x + b.width &&
                    a.x + a.width > b.x &&
                    a.y < b.y + b.height &&
                    a.y + a.height > b.y);
        }
        
        /**
         * Get collision info between two circles
         */
        inline CollisionInfo GetCircleCollisionInfo(RigidBody& a, RigidBody& b) {
            CollisionInfo info;
            
            Vector2 distance = Vector2Subtract(b.position, a.position);
            float distanceMag = Vector2Length(distance);
            float minDistance = a.radius + b.radius;
            
            if (distanceMag < minDistance) {
                info.hasCollision = true;
                info.penetration = minDistance - distanceMag;
                info.normal = Vector2Normalize(distance);
                info.bodyA = &a;
                info.bodyB = &b;
            }
            
            return info;
        }

        inline uint64_t HashGridCell(int x, int y) {
            return (((uint64_t)(uint32_t)x) << 32) | (uint32_t)y;
        }

        inline int GetGridCell(float value, float cellSize) {
            return (int)floorf(value / cellSize);
        }

        inline void InsertBodyToGrid(std::unordered_map<uint64_t, std::vector<size_t>>& grid,
                                     const RigidBody& body, size_t index, float cellSize) {
            int minCellX = GetGridCell(body.position.x - body.radius, cellSize);
            int maxCellX = GetGridCell(body.position.x + body.radius, cellSize);
            int minCellY = GetGridCell(body.position.y - body.radius, cellSize);
            int maxCellY = GetGridCell(body.position.y + body.radius, cellSize);

            for (int y = minCellY; y <= maxCellY; y++) {
                for (int x = minCellX; x <= maxCellX; x++) {
                    grid[HashGridCell(x, y)].push_back(index);
                }
            }
        }

        inline CollisionInfo GetCircleRectCollisionInfo(RigidBody& circle, Rectangle rect) {
            CollisionInfo info;

            Vector2 closestPoint = {
                fmaxf(rect.x, fminf(circle.position.x, rect.x + rect.width)),
                fmaxf(rect.y, fminf(circle.position.y, rect.y + rect.height))
            };

            Vector2 difference = Vector2Subtract(circle.position, closestPoint);
            float distance = Vector2Length(difference);
            float minDistance = circle.radius;

            if (distance < minDistance) {
                info.hasCollision = true;
                info.penetration = minDistance - distance;
                info.normal = (distance > 0) ? Vector2Normalize(difference) : Vector2{0, -1};
                info.bodyA = &circle;
                info.bodyB = nullptr;
            }

            return info;
        }

        inline void ResolveCircleRectCollision(RigidBody& body, const CollisionInfo& collision) {
            if (!collision.hasCollision || body.isStatic) return;

            // Push body out of collision along normal
            body.position = Vector2Add(body.position, Vector2Scale(collision.normal, collision.penetration));

            float velocityAlongNormal = Vector2DotProduct(body.velocity, collision.normal);
            if (velocityAlongNormal < 0.0f) {
                Vector2 normalVelocity = Vector2Scale(collision.normal, velocityAlongNormal);
                Vector2 tangentVelocity = Vector2Subtract(body.velocity, normalVelocity);

                // Reflect or slide along surface depending on collision orientation
                if (collision.normal.y < GROUND_NORMAL_THRESHOLD) {
                    // Ground collision: keep horizontal velocity and dampen vertical component
                    body.velocity = Vector2Scale(tangentVelocity, 1.0f);
                } else {
                    // Wall/ceiling collision: remove velocity component into the surface
                    body.velocity = tangentVelocity;
                }
            }

            if (collision.normal.y < GROUND_NORMAL_THRESHOLD) {
                body.isGrounded = true;
                body.velocity.x *= FRICTION;
            }
        }

        // --- COLLISION RESPONSE ---
        
        /**
         * Resolve collision between two rigid bodies
         */
        inline void ResolveCollision(CollisionInfo& collision) {
            if (!collision.hasCollision) return;
            
            RigidBody* bodyA = collision.bodyA;
            RigidBody* bodyB = collision.bodyB;
            
            // Don't resolve if both are static
            if (bodyA->isStatic && bodyB->isStatic) return;
            
            // Calculate relative velocity
            Vector2 relativeVelocity = Vector2Subtract(bodyB->velocity, bodyA->velocity);
            float velocityAlongNormal = Vector2DotProduct(relativeVelocity, collision.normal);
            
            // Don't resolve if velocities are separating
            if (velocityAlongNormal > 0) return;
            
            // Calculate restitution (bounciness)
            float restitution = BOUNCE_DAMPING;
            float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
            impulseScalar /= (1.0f / bodyA->mass) + (1.0f / bodyB->mass);
            
            // Apply impulse
            Vector2 impulse = Vector2Scale(collision.normal, impulseScalar);
            
            if (!bodyA->isStatic) {
                bodyA->velocity = Vector2Subtract(bodyA->velocity, 
                    Vector2Scale(impulse, 1.0f / bodyA->mass));
            }
            
            if (!bodyB->isStatic) {
                bodyB->velocity = Vector2Add(bodyB->velocity, 
                    Vector2Scale(impulse, 1.0f / bodyB->mass));
            }
            
            // Position correction (prevent sinking)
            float percent = 0.2f; // 20% correction
            float slop = 0.01f; // penetration tolerance
            Vector2 correction = Vector2Scale(collision.normal, 
                fmaxf(collision.penetration - slop, 0.0f) / (1.0f / bodyA->mass + 1.0f / bodyB->mass) * percent);
            
            if (!bodyA->isStatic) {
                bodyA->position = Vector2Subtract(bodyA->position, 
                    Vector2Scale(correction, 1.0f / bodyA->mass));
            }
            
            if (!bodyB->isStatic) {
                bodyB->position = Vector2Add(bodyB->position, 
                    Vector2Scale(correction, 1.0f / bodyB->mass));
            }
        }
        
        // --- GROUND DETECTION ---
        
        /**
         * Check if body is on ground
         */
        inline bool IsGrounded(const RigidBody& body, Rectangle ground) {
            Rectangle checkRect = {
                body.position.x - body.radius,
                body.position.y + body.radius - 5,
                body.radius * 2,
                10
            };
            return CheckRectCollision(checkRect, ground);
        }
        
        /**
         * Apply ground friction to body
         */
        inline void ApplyGroundFriction(RigidBody& body) {
            if (body.isGrounded) {
                body.velocity.x *= FRICTION;
            }
        }
        
        // --- MOVEMENT HELPERS ---
        
        /**
         * Move body with collision detection against static rectangle
         */
        inline void MoveWithCollision(RigidBody& body, Vector2 movement, Rectangle obstacle) {
            if (body.isStatic) return;
            
            // Horizontal movement with sliding
            body.position.x += movement.x;
            if (CheckCircleRectCollision(body, obstacle)) {
                body.position.x -= movement.x;
                body.velocity.x = 0;
            }
            
            // Vertical movement with sliding
            body.position.y += movement.y;
            if (CheckCircleRectCollision(body, obstacle)) {
                body.position.y -= movement.y;
                body.velocity.y = 0;
                
                if (movement.y > 0) {
                    body.isGrounded = true;
                }
            }
        }
        
        /**
         * Simple platformer movement
         */
        inline void PlatformerMovement(RigidBody& body, float speed, float jumpForce, Rectangle ground) {
            // Horizontal movement
            if (IsKeyDown(KEY_RIGHT)) {
                ApplyForce(body, {speed * 100, 0});
            }
            if (IsKeyDown(KEY_LEFT)) {
                ApplyForce(body, {-speed * 100, 0});
            }
            
            // Jump
            if (IsKeyPressed(KEY_SPACE) && body.isGrounded) {
                ApplyImpulse(body, {0, -jumpForce});
                body.isGrounded = false;
            }
            
            // Apply ground friction
            ApplyGroundFriction(body);
            
            // Check if grounded
            body.isGrounded = IsGrounded(body, ground);
        }
        
        // --- ADVANCED PHYSICS ---
        
        /**
         * Apply spring force between two bodies
         */
        inline void ApplySpringForce(RigidBody& bodyA, RigidBody& bodyB, 
                                  float restLength, float stiffness) {
            Vector2 direction = Vector2Subtract(bodyB.position, bodyA.position);
            float distance = Vector2Length(direction);
            
            if (distance > 0) {
                direction = Vector2Normalize(direction);
                float force = stiffness * (distance - restLength);
                Vector2 springForce = Vector2Scale(direction, force);
                
                ApplyForce(bodyA, springForce);
                ApplyForce(bodyB, Vector2Negate(springForce));
            }
        }
        
        /**
         * Apply gravitational attraction between two bodies
         */
        inline void ApplyGravity(RigidBody& bodyA, RigidBody& bodyB, float strength) {
            Vector2 direction = Vector2Subtract(bodyB.position, bodyA.position);
            float distance = Vector2Length(direction);
            
            if (distance > 0) {
                direction = Vector2Normalize(direction);
                float force = (strength * bodyA.mass * bodyB.mass) / (distance * distance);
                Vector2 gravityForce = Vector2Scale(direction, force);
                
                ApplyForce(bodyA, gravityForce);
                ApplyForce(bodyB, Vector2Negate(gravityForce));
            }
        }
        
        /**
         * Simple drag force (air resistance)
         */
        inline void ApplyDrag(RigidBody& body, float dragCoefficient) {
            Vector2 dragForce = Vector2Scale(body.velocity, -dragCoefficient);
            ApplyForce(body, dragForce);
        }
        
        // --- PHYSICS WORLD ---
        
        class PhysicsWorld {
        private:
            std::vector<RigidBody> bodies;
            std::vector<Rectangle> staticColliders;
            
        public:
            void AddBody(const RigidBody& body) {
                bodies.push_back(body);
            }
            
            void AddStaticCollider(Rectangle rect) {
                staticColliders.push_back(rect);
            }
            
            void Update(float deltaTime) {
                const float gridCellSize = 128.0f;
                std::unordered_map<uint64_t, std::vector<size_t>> grid;
                grid.reserve(bodies.size() * 2);

                // Update all bodies and resolve static collisions
                for (auto& body : bodies) {
                    body.isGrounded = false;
                    UpdateRigidBody(body, deltaTime);

                    for (const auto& collider : staticColliders) {
                        CollisionInfo collision = GetCircleRectCollisionInfo(body, collider);
                        if (collision.hasCollision) {
                            ResolveCircleRectCollision(body, collision);
                        }
                    }
                }

                // Build spatial hash grid for body-body collision broad phase
                for (size_t i = 0; i < bodies.size(); i++) {
                    InsertBodyToGrid(grid, bodies[i], i, gridCellSize);
                }

                // Check body-to-body collisions using neighboring cells
                for (size_t i = 0; i < bodies.size(); i++) {
                    int cellX = GetGridCell(bodies[i].position.x, gridCellSize);
                    int cellY = GetGridCell(bodies[i].position.y, gridCellSize);

                    for (int offsetY = -1; offsetY <= 1; offsetY++) {
                        for (int offsetX = -1; offsetX <= 1; offsetX++) {
                            uint64_t key = HashGridCell(cellX + offsetX, cellY + offsetY);
                            auto it = grid.find(key);
                            if (it == grid.end()) continue;

                            for (size_t j : it->second) {
                                if (j <= i) continue;
                                if (CheckCircleCollision(bodies[i], bodies[j])) {
                                    CollisionInfo info = GetCircleCollisionInfo(bodies[i], bodies[j]);
                                    ResolveCollision(info);
                                }
                            }
                        }
                    }
                }
            }
            
            void Clear() {
                bodies.clear();
                staticColliders.clear();
            }
            
            const std::vector<RigidBody>& GetBodies() const {
                return bodies;
            }
            
            const std::vector<Rectangle>& GetStaticColliders() const {
                return staticColliders;
            }
        };
    }
}

#endif
