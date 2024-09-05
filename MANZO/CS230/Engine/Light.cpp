#include "raylib.h"
#include <cmath>
#include <vector>
#include <iostream>
#include "Light.h"


Math::vec2 LightPosition;
std::vector<Math::vec2> ShadowPoint;
std::vector<std::vector<Math::vec2>> ShadowObject;
const Color shadowcolor = BLUE;
std::vector<Vector2> CollisionPoint;
const float shadowPointradius = 5.0f;

Math::vec2 findCollinearPoint(Math::vec2 a, Math::vec2 b, bool fromA = true) {
    Math::vec2 c;
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    c = { b.x + dx * GetScreenWidth(),b.y + dy * GetScreenHeight() };

    return c;
}



void CS230::Light::AddShadowPoint(Math::vec2 position)
{
    ShadowPoint.push_back(position);
}

void CS230::Light::AddshadowObject()
{
    if (ShadowPoint.size() != 0) {
        ShadowObject.push_back(ShadowPoint);
        ShadowPoint.clear();
    }
}

void CS230::Light::Update(Math::vec2 lightPosition, Math::vec2 shadowpoint, Math::vec2 Collinearpoint, Math::vec2 nextCollinearpoint)
{
    if (lightPosition.x > shadowpoint.x || lightPosition.y > shadowpoint.y) {
        DrawTriangle({ (float)shadowpoint.x,(float)shadowpoint.y }, { (float)Collinearpoint.x,(float)Collinearpoint.y }, { (float)nextCollinearpoint.x,(float)nextCollinearpoint.y }, shadowcolor);
    }
    if (lightPosition.x > shadowpoint.x || lightPosition.y < shadowpoint.y) {
        DrawTriangle({ (float)shadowpoint.x,(float)shadowpoint.y }, { (float)Collinearpoint.x,(float)Collinearpoint.y }, { (float)nextCollinearpoint.x,(float)nextCollinearpoint.y }, shadowcolor);
    }
    if (lightPosition.x < shadowpoint.x || lightPosition.y > shadowpoint.y) {
        DrawTriangle({ (float)shadowpoint.x,(float)shadowpoint.y }, { (float)nextCollinearpoint.x,(float)nextCollinearpoint.y }, { (float)Collinearpoint.x,(float)Collinearpoint.y }, shadowcolor);
    }
    if (lightPosition.x < shadowpoint.x || lightPosition.y < shadowpoint.y) {
        DrawTriangle({ (float)shadowpoint.x,(float)shadowpoint.y }, { (float)nextCollinearpoint.x,(float)nextCollinearpoint.y }, { (float)Collinearpoint.x,(float)Collinearpoint.y }, shadowcolor);
    }
}

void CS230::Light::AddLightPosition(Math::vec2 position) {
    LightPosition = position;
    DrawCircle(position.x, position.y, 150, { 255,255,255,100 });
}

void CS230::Light::Draw(float light_size)
{
    if (ShadowObject.size() != 0) {
        for (int i = 0; i < ShadowObject.size(); ++i) {
            std::vector<Math::vec2>& shape = ShadowObject[i];
            for (int j = 0; j < shape.size(); ++j) {
                if (CheckCollisionCircles({ (float)LightPosition.x,(float)LightPosition.y }, light_size, { (float)shape[j].x,(float)shape[j].y }, shadowPointradius)) {

                    //shadow position
                    //DrawCircleV(shape[j], shadowPointradius, BLUE);
                    //shadowline
                    //DrawLineV(LightPosition, findCollinearPoint(LightPosition, shape[j]), WHITE);
                    if (j != 0) {
                        Math::vec2 nextPoint = (j + 1 < shape.size()) ? shape[j + 1] : shape.front();
                        Update(LightPosition, shape[j], findCollinearPoint(LightPosition, shape[j]), findCollinearPoint(LightPosition, shape[j - 1]));
                        Update(LightPosition, shape[j], findCollinearPoint(LightPosition, shape[j]), findCollinearPoint(LightPosition, nextPoint));

                        //more shadow
                        DrawTriangle({ (float)findCollinearPoint(LightPosition, shape[j]).x, (float)findCollinearPoint(LightPosition, shape[j]).y }, { (float)shape[j - 1].x,(float)shape[j - 1].y }, { (float)nextPoint.x,(float)nextPoint.y }, shadowcolor);

                    }
                    else if (j == 0) {
                        Math::vec2 nextPoint = (j + 1 < shape.size()) ? shape[j + 1] : shape.front();
                        Update(LightPosition, shape[j], findCollinearPoint(LightPosition, nextPoint), findCollinearPoint(LightPosition, shape[j]));
                        Update(LightPosition, shape[j], findCollinearPoint(LightPosition, shape.back()), findCollinearPoint(LightPosition, shape.front()));

                        //more shadow
                        DrawTriangle({ (float)findCollinearPoint(LightPosition, shape[j]).x, (float)findCollinearPoint(LightPosition, shape[j]).y }, { (float)shape.back().x,(float)shape.back().y }, { (float)shape[j + 1].x,(float)shape[j + 1].y }, shadowcolor);

                    }
                    //more shadow
                    if (j == shape.size() - 1) {
                        DrawTriangle({ (float)findCollinearPoint(LightPosition, shape[j]).x, (float)findCollinearPoint(LightPosition, shape[j]).y }, { (float)shape.front().x,(float)shape.front().y }, { (float)shape[j - 1].x,(float)shape[j - 1].y }, shadowcolor);
                    }
                }
            }
        }
    }
}