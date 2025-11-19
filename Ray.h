#pragma once

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <deque>
#include <iostream>
#include <math.h>

float PixelPerMeter = 100;

class BRay {
  private:
    float ray_distance;
    Vector2 Start_Position;
    std::deque<Vector2> lines;

    float Initial_Power;

    float direction_to_wall_normal;
    float best_direction_to_wall_normal;


  public:
    Color color = {255, 0, 0, 255};

    Vector2 Position;
    Vector2 Next_Position;
    Vector2 Direction;
    float speed;

    float frequency;
    float wavelength;

    float Power;

    bool reflected;

    BRay(Vector2 Position, Vector2 Direction, float speed, float frequency ,float Power){
      this->Position = Position;
      this->Direction = Vector2Normalize(Direction);
      this->speed = speed;
      this->Next_Position = Position + this->Direction*speed;
      this->Start_Position = Position;
      lines.push_back(Position);

      this->ray_distance = 1e-6f;

      this->wavelength = 0.125;

      this->Power = Power;
      this->Initial_Power = Power;

      this-> reflected = false;
    }

    float t = 0;

    void draw(bool Draw_Lines) {
      if (Draw_Lines == true) {
        for(int i = 0; i < lines.size() - 1; i++){ // Previous Lines
          DrawLineEx(lines[i], lines[i+1], 1, color);
        }
        DrawLineEx(Start_Position, Position, 1, color); // Current Line Path
      }

      DrawCircleV(Position, 2, color);

    }

    bool ComputeHitAgainstWall(const Vector2& curPos, const Vector2& moveVec,
                               const Vector2& a, const Vector2& b, const Vector2& wallNormal,
                               float &outHitT, Vector2 &outHitPos)
    {
        Vector2 nextPos = Vector2Add(curPos, moveVec);

        Vector2 ab = Vector2Subtract(b, a);

        Vector2 ap = Vector2Subtract(curPos, a);
        Vector2 apn = Vector2Subtract(nextPos, a);

        float t  = std::clamp(Vector2DotProduct(ap, ab) / Vector2DotProduct(ab, ab), 0.0f, 1.0f);
        float tn = std::clamp(Vector2DotProduct(apn, ab) / Vector2DotProduct(ab, ab), 0.0f, 1.0f);

        Vector2 Closest  = Vector2Add(a, Vector2Scale(ab, t));
        Vector2 Closestn = Vector2Add(a, Vector2Scale(ab, tn));

        float sign  = ((ab.x * ap.y  - ab.y * ap.x)  < 0.0f) ? -1.0f : 1.0f;
        float signn = ((ab.x * apn.y - ab.y * apn.x) < 0.0f) ? -1.0f : 1.0f;

        float distance  = Vector2Distance(curPos, Closest)  * sign;
        float distancen = Vector2Distance(nextPos, Closestn) * signn;

        direction_to_wall_normal = distancen;

        // if they have opposite signs -> crossing occurred (or one exactly zero)
        if (distance * distancen < 0.0f) {
            // compute interpolation factor along moveVec
            float denom = (distance - distancen);
            if (denom < 1e-6 and denom > -1e-6) return false; // numerically degenerate

            float hitFactor = distance / denom; // between 0 and 1 if crossing along the segment
            if (hitFactor < 0.0f || hitFactor > 1.0f) return false;

            Vector2 hitPoint = Vector2Add(curPos, Vector2Scale(moveVec, hitFactor));
            float proj = Vector2DotProduct(Vector2Subtract(hitPoint, a), ab) / Vector2DotProduct(ab, ab);
            if (proj < 0.0f || proj > 1.0f) return false;

            outHitT = hitFactor;
            outHitPos = hitPoint;
            return true;
        }

        return false;
    }

    void ResolveCollisions(const std::vector<Wall>& walls, float delta) {

        Vector2 remainingMove = Vector2Scale(Direction, speed * delta);

        const int MAX_ITER = 8;     // allow up to N reflections per frame
        const float kepsilon = 0.001f; // small nudge to avoid re-colliding same wall
        reflected = false;

        for (int iter = 0; iter < MAX_ITER; ++iter) {
            // If remaining movement is almost zero, finish
            float remLen = Vector2Length(remainingMove);
            if (remLen <= 1e-6f) break;

            // Find earliest hit among all walls for this remainingMove
            bool foundHit = false;
            float bestT = 1.0f + 1e-6f; // minimal t in (0..1]
            int bestWallIdx = -1;
            Vector2 bestHitPos = {0,0};

            for (int i = 0; i < (int)walls.size(); ++i) {
                float hitT;
                Vector2 hitPos;
                if (ComputeHitAgainstWall(Position, remainingMove, walls[i].Start, walls[i].End, walls[i].Normal, hitT, hitPos)) {
                    if (hitT < bestT) {
                        bestT = hitT;
                        bestWallIdx = i;
                        bestHitPos = hitPos;
                        foundHit = true;

                        best_direction_to_wall_normal = direction_to_wall_normal;
                        Start_Position = hitPos;
                        Initial_Power *= 0.8;
                        lines.push_back(hitPos);
                    }
                }
            }


            if (!foundHit) {
                // No collision: consume the remaining movement and finish
                Position = Vector2Add(Position, remainingMove);
                // Next_Position kept as future predicted step
                remainingMove = {0,0};
                break;
            }

            // There is a hit: step to hit point
            Vector2 moveToHit = Vector2Scale(remainingMove, bestT);
            Position = Vector2Add(Position, moveToHit);

            // Reflect direction using the wall's normal
            Vector2 oldDir = Direction;
            Direction = Vector2Reflect(Direction, walls[bestWallIdx].Normal);

            // Compute remaining movement: remaining portion of remainingMove after hit
            float leftoverFactor = 1.0f - bestT;
            float totalLeftLen = Vector2Length(remainingMove) * leftoverFactor;

            // New remainingMove is along the reflected direction keeping the leftover length
            remainingMove = Vector2Scale(Vector2Normalize(Direction), totalLeftLen);

           // Nudge the position slightly along the normal of the wall outward to avoid being numerically "inside" the wall
            if ( best_direction_to_wall_normal < 0 ) {
              Position = Vector2Add(Position, Vector2Scale(walls[bestWallIdx].Normal, kepsilon) );
            }
            else if ( best_direction_to_wall_normal > 0 ) {
              Position = Vector2Add(Position, Vector2Negate( Vector2Scale(walls[bestWallIdx].Normal, kepsilon) ) );
            }

            // continue the loop to check for further collisions with the rest of the remainingMove
        }

        // Update Next_Position as the predicted next step using the (possibly changed) Direction.
        Next_Position = Vector2Add(Position, Vector2Scale(Direction, speed * delta));

        ray_distance += Vector2Distance(Next_Position, Position)/PixelPerMeter;

        Power = Initial_Power * pow( wavelength/(4*M_PI*ray_distance) ,2);

        color.a = (int)fmin(fmax((10*log10(Power/0.001) + 90)/110 * 255.0, 0.0), 255.0);
    }

};
