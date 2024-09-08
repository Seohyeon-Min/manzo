#pragma once
#include "GameObject.h"
#include "GameObjectManager.h"
#include "../Game/GameObjectTypes.h"

namespace CS230 {
	class Particle : public GameObject {
	public:
		Particle(const std::filesystem::path& sprite_file);
		void Start(Math::vec2 position, Math::vec2 velocity, double max_life);
		void Update(double dt) override;
		void Draw(Math::TransformationMatrix camera_matrix) override;
		bool Alive() {
			return  life > 0;
		}
		GameObjectTypes Type() { return GameObjectTypes::Particle; }
	private:
		double life;
		Sprite sprite;
	};

	template<typename T>
	class ParticleManager : public Component {
	public:
		ParticleManager();
		~ParticleManager();
		void Emit(int count, Math::vec2 emitter_position, Math::vec2 emitter_velocity, Math::vec2 direction, double spread);
	private:
		std::vector<T*> particles;
		int index;
	};

	template<typename T>
	inline ParticleManager<T>::ParticleManager() : index(0)
	{
		int count = T::MaxCount;
		for (int i = 0; i < count; i++)
		{
			T* particle = new T();
			particles.push_back(particle);
			Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(particle);
		}
	}

	template<typename T>
	inline ParticleManager<T>::~ParticleManager()
	{
		particles.clear();
	}



	template<typename T>
	void ParticleManager<T>::Emit(int count, Math::vec2 emitter_position, Math::vec2 emitter_velocity, Math::vec2 direction, double spread)
	{
		//Loop to make sure we can create multiple particles (count)
			//Check if the current particle is alive
				//Log "Particle overwritten"
		for (int i = 0; i < count; i++)
		{
			if (particles[i]->Alive())
			{
				Engine::GetLogger().LogError("Particle overwritten");
			}
		}

		double angle_variation = 0.0;
		//If spread is not 0
		if (spread != 0)
		{
			angle_variation = ((rand() % static_cast<int>(spread * 1024)) / 1024.0f) - spread / 2;
		}

		Math::vec2 random_magnitude = direction * (((rand() % 1024) / 2048.0f) + 0.5f);
		Math::vec2 particle_velocity = Math::RotationMatrix(angle_variation) * random_magnitude + emitter_velocity;
		//particles[index]->Start()
		particles[index]->Start(emitter_position, emitter_velocity, T::MaxLife);

		index++;
		//Advance index by 1
		if (index >= count)
		{
			index = 0;
		}
		//Check if index is outside of range, and if it is, set it to 0
	}
}
