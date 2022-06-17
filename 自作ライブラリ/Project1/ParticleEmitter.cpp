#include "ParticleEmitter.h"
#include "Matrix4.h"

#include<random>

ParticleManager* ParticleEmitter::particleManager = nullptr;

void ParticleEmitter::Initialize()
{
    particleManager = ParticleManager::GetInstance();
    particleManager->Initialize();
}

void ParticleEmitter::CutEffect(const Vector3& arg_pos, const Vector3& arg_vec)
{
    for (int i = 0; i < 75; i++)
    {
        //”ò‚ñ‚Å‚¢‚­•ûŒü
        Vector2 vecRange = { -0.15f,0.15f };
        float vecRangeX = GetRandom(vecRange.x, vecRange.y);
        float vecRangeZ = GetRandom(vecRange.x, vecRange.y);

        Vector3 vel = { (arg_vec.x + vecRangeX), arg_vec.y, (arg_vec.z + vecRangeZ) };
        vel *= -1.0f;
        vel *= GetRandom(1.0f, 5.0f) * 0.5f;
        vel.y = GetRandom(0.5f, 1.0f);

        Vector3 acc = { 0.0f,-0.1f,0.0f };

        //”­¶”ÍˆÍ
        Vector3 vec = Vector3::Normalize(arg_vec);
        float range = GetRandom(-1.0f, 0.0f);
        Vector3 position = arg_pos + (vec * range);

        Particle* particle = new Particle();

        particle->parameter.accel = acc;
        particle->parameter.num_frame = 5;
        particle->parameter.s_alpha = 1.0f;
        particle->parameter.e_alpha = 1.0f;
        particle->parameter.scaleAce = { 0,0 };
        particle->parameter.s_scale = 0.5f;
        particle->parameter.e_scale = 0.5f;
        particle->parameter.s_color = { (255.0f / 255.0f),  (100.0f / 255.0f),  (1.0f / 255.0f) };
        particle->parameter.e_color = { (255.0f / 255.0f),  (1.0f / 255.0f),  (1.0f / 255.0f) };
        particle->parameter.s_rotation = { 0,0,0 };
        particle->parameter.e_rotation = { 0,0,0 };

        particle->vsParam.position = position;
        particle->vsParam.velocity = vel;
        particle->vsParam.rotation = Vector3(0, 0, 0);
        particle->vsParam.scale = 0.25f;
        particle->vsParam.scaleVel = (particle->parameter.e_scale - particle->parameter.s_scale) / particle->parameter.num_frame;
        particle->vsParam.billboardActive = 1;
        particle->vsParam.color = { (255.0f / 255.0f),  (100.0f / 255.0f),  (1.0f / 255.0f), 1.0f };
        particle->vsParam.frame = 0;
        particle->vsParam.isDead = false;

        particleManager->Add(particle, "particle");
    }
    for (int i = 0; i < 25; i++)
    {
        //”ò‚ñ‚Å‚¢‚­•ûŒü
        Vector2 vecRange = { -0.15f,0.15f };
        float vecRangeX = GetRandom(vecRange.x, vecRange.y);
        float vecRangeZ = GetRandom(vecRange.x, vecRange.y);

        Vector3 vel = { (arg_vec.x + vecRangeX), arg_vec.y, (arg_vec.z + vecRangeZ) };
        vel *= -1.0f;
        vel *= GetRandom(1.0f, 5.0f) * 0.5f;
        vel.y = GetRandom(0.5f, 1.0f);

        Vector3 acc = { 0.0f,-0.1f,0.0f };

        //”­¶”ÍˆÍ
        Vector3 vec = Vector3::Normalize(arg_vec);
        float range = GetRandom(-1.0f, 0.0f);
        Vector3 position = arg_pos + (vec * range);

        Particle* particle = new Particle();

        particle->parameter.accel = acc;
        particle->parameter.num_frame = 3;
        particle->parameter.s_alpha = 1.0f;
        particle->parameter.e_alpha = 1.0f;
        particle->parameter.scaleAce = { 0,0 };
        particle->parameter.s_scale = 0.5f;
        particle->parameter.e_scale = 0.5f;
        particle->parameter.s_color = { (255.0f / 255.0f),  (255.0f / 255.0f),  (255.0f / 255.0f) };
        particle->parameter.e_color = { (255.0f / 255.0f),  (255.0f / 255.0f),  (255.0f / 255.0f) };
        particle->parameter.s_rotation = { 0,0,0 };
        particle->parameter.e_rotation = { 0,0,0 };

        particle->vsParam.position = position;
        particle->vsParam.velocity = vel;
        particle->vsParam.rotation = Vector3(0, 0, 0);
        particle->vsParam.scale = 0.25f;
        particle->vsParam.scaleVel = (particle->parameter.e_scale - particle->parameter.s_scale) / particle->parameter.num_frame;
        particle->vsParam.billboardActive = 1;
        particle->vsParam.color = { (255.0f / 255.0f),  (255.0f / 255.0f),  (255.0f / 255.0f), 1.0f };
        particle->vsParam.frame = 0;
        particle->vsParam.isDead = false;

        particleManager->Add(particle, "particle");
    }
}

void ParticleEmitter::ShockEffect(const Vector3& arg_pos, const Vector3& arg_color)
{
    int amount = 75;
    float addangle = (float)(360.0f / (float)(amount));
    float angle = 0.0f;

    for (int i = 0; i < amount; i++)
    {
        Particle* particle = new Particle();

        particle->parameter.accel = Vector3(0, 0, 0);
        particle->parameter.num_frame = 20;
        particle->parameter.s_alpha = 1.0f;
        particle->parameter.e_alpha = 0.0f;
        particle->parameter.scaleAce = { 0,0 };
        particle->parameter.s_scale = 0.0f;
        particle->parameter.e_scale = 1.0f;
        particle->parameter.s_color = { (arg_color.x / 255.0f),  (arg_color.y / 255.0f),  (arg_color.z / 255.0f) };
        particle->parameter.e_color = { (arg_color.x / 255.0f),  (arg_color.y / 255.0f),  (arg_color.z / 255.0f) };
        particle->parameter.s_rotation = { 0,0,0 };
        particle->parameter.e_rotation = { 0,0,0 };

        particle->vsParam.position = arg_pos;
        float radius = angle * 3.14f / 180.0f;
        angle += addangle;
        particle->vsParam.velocity = Vector3::Normalize({ cosf(radius), 0.0f, sinf(radius) }) * 0.25f;

        particle->vsParam.rotation = Vector3(0, 0, 0);
        particle->vsParam.scale = 0.0f;
        particle->vsParam.scaleVel = (particle->parameter.e_scale - particle->parameter.s_scale) / particle->parameter.num_frame;
        particle->vsParam.billboardActive = 1;
        particle->vsParam.color = { (arg_color.x / 255.0f),  (arg_color.y / 255.0f),  (arg_color.z / 255.0f), 1.0f };
        particle->vsParam.frame = 0;
        particle->vsParam.isDead = false;

        particleManager->Add(particle, "particle");
    }
}

float ParticleEmitter::GetRandom(float arg_min, float arg_max)
{
    std::random_device rnd;
    std::mt19937_64 mt64(rnd());
    std::uniform_real_distribution<float> genRandFloat(arg_min, arg_max);
    return genRandFloat(mt64);
}