#include "util.h"

#include "glm/glm.hpp"
using namespace glm;

#include <cmath>

bool rayIntersectsSphere(vec2 point1, vec2 point2, vec2 sphere, float radius, vec2 &intersect)
{
    float a,b,c;
    float bb4ac;
    vec2 dp;
    float mu[2];
    vec2 intersects[2];
    float dist[2];
    float closestDist;
    float maxDist;

    bool result = false;

    dp.x = point2.x - point1.x;
    dp.y = point2.y - point1.y;
    a = dp.x * dp.x + dp.y * dp.y;
    b = 2 * (dp.x * (point1.x - sphere.x) + dp.y * (point1.y - sphere.y));
    c = sphere.x * sphere.x + sphere.y * sphere.y;
    c += point1.x * point1.x + point1.y * point1.y;
    c -= 2 * (sphere.x * point1.x + sphere.y * point1.y);
    c -= radius * radius;
    bb4ac = b * b - 4 * a * c;

    if(bb4ac >= 0.0)
    {
        mu[0] = (-b + sqrt(bb4ac)) / (2.0 * a);
        mu[1] = (-b - sqrt(bb4ac)) / (2.0 * a);

        intersects[0] = point1 + ((point2 - point1) * mu[0]);
        intersects[1] = point1 + ((point2 - point1) * mu[1]);

        dist[0] = length(point1 - intersects[0]);
        dist[1] = length(point1 - intersects[1]);
        closestDist = 0.0;

        if(dist[0] < dist[1])
        {
            closestDist = dist[0];
            intersect = intersects[0];
        }
        else
        {
            closestDist = dist[1];
            intersect = intersects[1];
        }

		maxDist = length(point1 - point2);
		if(closestDist <= maxDist)
		{
			result = true;
		}
    }

   return result;
}
