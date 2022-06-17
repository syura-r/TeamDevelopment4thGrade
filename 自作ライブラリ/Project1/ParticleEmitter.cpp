#include "ParticleEmitter.h"
#include "Matrix4.h"

#include<random>

ParticleManager* ParticleEmitter::particleManager = nullptr;

void ParticleEmitter::Initialize()
{
    particleManager = ParticleManager::GetInstance();
    particleManager->Initialize();
}

void ParticleEmitter::CreateExplosion(const Vector3& pos)
{
    for (int i = 0; i < 30; i++)
    {
        Particle* particle = new Particle();
        Vector3 position = pos;
        float alpha = 1.0f;

        Vector3 rotation = { 0,0,0 };

        particle->vsParam.frame = 0;
        particle->parameter.num_frame = 60;

        Vector3 velocity = { 0,0,0 };
        particle->parameter.accel = { (std::rand() % 100 - 50) * 0.01f,(std::rand() % 100 - 50) * 0.01f,(std::rand() % 100 - 50) * 0.01f };

        float scale = 1.0f;
        particle->parameter.s_scale = 1.0f;
        particle->parameter.e_scale = 5.0f;
        //線形補間
        particle->vsParam.scaleVel = (particle->parameter.e_scale - particle->parameter.s_scale) / particle->parameter.num_frame;


        Vector3 color = { 1,1,1 };
        particle->parameter.s_color = { 0,0,0 };
        particle->parameter.e_color = { (std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f) };

        particle->vsParam.isDead = false;

        XMMATRIX mat = { position.x,position.y,position.z,0,
                        rotation.x,rotation.y,rotation.z,0,
                        scale,velocity.x,velocity.y ,velocity.z ,
                        color.x,color.y,color.z,alpha };

        particle->vsParam.position = position;
        particle->vsParam.rotation = rotation;
        particle->vsParam.velocity = velocity;
        particle->vsParam.scale = scale;
        particle->vsParam.color = { color.x,color.y,color.z,alpha };
        particle->vsParam.billboardActive = 1;

        particleManager->Add(particle, "particle");
    }
}

void ParticleEmitter::CreateShock(const Vector3& pos, const Vector3& arg_rotation)
{
    //for (int i = 0; i < 3; i++)
    //{
    Particle* particle = new Particle();
    Vector3 position = pos;
    Vector3 rotation = arg_rotation;

    particle->vsParam.frame = 0;
    particle->parameter.num_frame = 25;
    //particle->parameter.num_frame = 60;

    Vector3 velocity = { 0,0,0 };
    particle->parameter.accel = { 0,0,0 };

    //particle->parameter.accel = { (std::rand() % 100 - 50) * 0.01f,(std::rand() % 100 - 50) * 0.01f,(std::rand() % 100 - 50) * 0.01f };

    particle->parameter.s_rotation = rotation;
    particle->parameter.e_rotation = rotation;


    float scale = 1.0f;
    particle->parameter.s_scale = 1.0f;
    particle->parameter.e_scale = 4.0f;
    //線形補間
    particle->vsParam.scaleVel = (particle->parameter.e_scale - particle->parameter.s_scale) / particle->parameter.num_frame;

    Vector3 color = { (std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f) };
    //Vector3 color = { 1,1,1 };

    particle->parameter.s_color = { 1,1,1 };
    //particle->parameter.e_color = { (std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f) };
    particle->parameter.e_color = { 0,0,0 };


    particle->vsParam.isDead = false;

    float alpha = 1.0f;

    particle->vsParam.position = position;
    particle->vsParam.rotation = rotation;
    particle->vsParam.velocity = velocity;
    particle->vsParam.scale = scale;
    particle->vsParam.color = { color.x,color.y,color.z,alpha };
    particle->vsParam.billboardActive = 0;
    particleManager->Add(particle, "shock");
    //}
}

void ParticleEmitter::CreateGetEffect(const Vector3& pos)
{
    for (int i = 0; i < 30; i++)
    {
        Particle* particle = new Particle();
        Vector3 position = pos;
        Vector3 rotation = { 0,0,0 };

        particle->vsParam.frame = 0;
        particle->parameter.num_frame = 35;
        //particle->parameter.num_frame = 60;

        Vector3 velocity = { (std::rand() % 100 - 50) * 0.0005f,std::rand() % 100 * -0.001f,(std::rand() % 100 - 50) * 0.0005f };
        particle->parameter.accel = { 0,0.005f,0 };

        //particle->parameter.accel = { (std::rand() % 100 - 50) * 0.01f,(std::rand() % 100 - 50) * 0.01f,(std::rand() % 100 - 50) * 0.01f };

        particle->parameter.s_rotation = { 0,0,std::rand() % 200 * 0.01f * (float)XM_PI };
        particle->parameter.e_rotation = { 0,0,std::rand() % 200 * 0.01f * (float)XM_PI };


        float scale = std::rand() % 100 * 0.01f;
        particle->parameter.s_scale = scale;
        particle->parameter.e_scale = 0.0f;
        //線形補間
        particle->vsParam.scaleVel = (particle->parameter.e_scale - particle->parameter.s_scale) / particle->parameter.num_frame;

        Vector3 color = { 0,1,0 };
        //Vector3 color = { 1,1,1 };

        particle->parameter.s_color = { 0,1,0 };
        //particle->parameter.e_color = { (std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f) };
        particle->parameter.e_color = { 0,0,0 };


        particle->vsParam.isDead = false;

        float alpha = 1.0f;

        particle->vsParam.position = position;
        particle->vsParam.rotation = rotation;
        particle->vsParam.velocity = velocity;
        particle->vsParam.scale = scale;
        particle->vsParam.color = { color.x,color.y,color.z,alpha };
        particle->vsParam.billboardActive = 1;
        particleManager->Add(particle, "starEffect");
    }
}

void ParticleEmitter::CreateRunDust(const Vector3& pos, const Vector3& direction)
{
    Vector3 dir = direction;
    if (dir.Length() == 0)
        return;
    Vector3 BaseDir = { 0,0,1 };
    int revers = 1;
    if (dir.Dot({ 1,0,0 }) < 0)
    {
        BaseDir = { 0,0,-1 };
        revers = -1;
    }
    const float dirRotY = acosf(dir.Dot(BaseDir));

    for (int i = 0; i < 5; i++)
    {
        Particle* particle = new Particle();
        Vector3 position = pos + Vector3{ (std::rand() % 100 - 50) * 0.003f,0,(std::rand() % 100 - 50) * 0.003f };
        Vector3 rotation = { 0,0,0 };

        particle->vsParam.frame = 0;
        particle->parameter.num_frame = 20;
        //particle->parameter.num_frame = 60;

        Vector3 velocity = { (std::rand() % 100 - 50) * 0.001f, -0.01f,(std::rand() % 100) * -0.0002f };
        velocity = revers * velocity * XMMatrixRotationAxis({ 0,1,0 }, dirRotY);

        particle->parameter.accel = { 0,0.002f,0 };

        //particle->parameter.accel = { (std::rand() % 100 - 50) * 0.01f,(std::rand() % 100 - 50) * 0.01f,(std::rand() % 100 - 50) * 0.01f };

        particle->parameter.s_rotation = { 0,0,0 };
        particle->parameter.e_rotation = { 0,0,0 };


        float scale = 0.25f;
        particle->parameter.s_scale = scale;
        particle->parameter.e_scale = scale * 8;
        //線形補間
        particle->vsParam.scaleVel = (particle->parameter.e_scale - particle->parameter.s_scale) / particle->parameter.num_frame;

        Vector3 color = 0.03f;
        //Vector3 color = { 1,1,1 };

        particle->parameter.s_color = color;
        //particle->parameter.e_color = { (std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f),(std::rand() % 100 * 0.01f) };
        particle->parameter.e_color = { 0,0,0 };


        particle->vsParam.isDead = false;

        float alpha = 0.01f;

        particle->vsParam.position = position;
        particle->vsParam.rotation = rotation;
        particle->vsParam.velocity = velocity;
        particle->vsParam.scale = scale;
        particle->vsParam.color = { color.x,color.y,color.z,alpha };
        particle->vsParam.billboardActive = 1;
        particleManager->Add(particle, "particle");
    }
}

void ParticleEmitter::CutEffect(const Vector3& arg_pos, const Vector3& arg_vec)
{
    for (int i = 0; i < 5; i++)
    {
        //加速度
        Vector3 acc = { (std::rand() % 100 - 50) * 0.005f,(std::rand() % 100 - 50) * 0.005f,(std::rand() % 100 - 50) * 0.005f };

        //進行方向から火花の範囲計算
        Vector3 vec = Vector3::Normalize(arg_vec);
        float range = GetRandom(-10.0f, 10.0f);

        Vector3 position = arg_pos + (vec * range);

        Particle* particle = new Particle();

        particle->parameter.accel = acc;
        particle->parameter.num_frame = 30;
        particle->parameter.s_alpha = 1.0f;
        particle->parameter.e_alpha = 1.0f;
        particle->parameter.scaleAce = { 0,0 };
        particle->parameter.s_scale = 0.5f;
        particle->parameter.e_scale = 0.0f;
        particle->parameter.s_color = { 255.0f, 1, 1.0f };
        particle->parameter.e_color = { 255.0f, 1, 1.0f };
        particle->parameter.s_rotation = { 0,0,0 };
        particle->parameter.e_rotation = { 0,0,0 };

        particle->vsParam.position = position;
        particle->vsParam.velocity = Vector3(0, 0, 0);
        particle->vsParam.rotation = Vector3(0, 0, 0);
        particle->vsParam.scale = 0.5f;
        particle->vsParam.scaleVel = (particle->parameter.e_scale - particle->parameter.s_scale) / particle->parameter.num_frame;
        particle->vsParam.billboardActive = 1;
        particle->vsParam.color = { 255.0f, 1, 1.0f, 1.0f };
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