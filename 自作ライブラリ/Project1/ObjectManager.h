#pragma once
#include <memory>
#include <unordered_map>

class Object;
class TextureResource;
class ObjectManager
{
public:
	static ObjectManager* GetInstance();
	void CreateLineMap();

	void Add(Object* object = nullptr, bool preDraw = true);
	void AddExecute(Object* object = nullptr, bool preDraw = true);
	void Initialize();
	void Update();
	void Remove(Object* object);
	//
	void AddObjectsAtOnce();
	void RemoveDeadObjects();
	//�`�揀��
	void DrawReady();
	void PreDraw();
	void PostDraw();
private:
	ObjectManager() = default;
	ObjectManager(const ObjectManager&) = delete;
	~ObjectManager() = default;
	ObjectManager& operator=(const ObjectManager&) = delete;

	std::unordered_map<bool,std::vector<std::unique_ptr<Object>>>objects;
	std::unordered_map<bool, std::unordered_map<std::string, std::vector<Object*>>>drawObjects;

	std::unordered_map<bool, std::vector<Object*>> addScheduledObjects;
	std::unique_ptr <TextureResource> lineMap = nullptr;

};

