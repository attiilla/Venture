cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "venture")
project(${PROJECT_NAME})

find_package(SDL2 2.32.2 EXACT REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(${PROJECT_NAME}
        Source/Main.cpp
        Source/Math.cpp
        Source/Random.cpp
        Source/Actors/Actor.cpp
        Source/Components/Component.cpp
        Source/Game.cpp
        Source/Components/DrawComponents/DrawComponent.cpp
        Source/Components/DrawComponents/DrawComponent.h
        Source/Components/RigidBodyComponent.cpp
        Source/Components/RigidBodyComponent.h
        Source/Components/ColliderComponents/CircleColliderComponent.cpp
        Source/Components/ColliderComponents/CircleColliderComponent.h
        Source/Json.h
        Source/CSV.h
        Source/Components/DrawComponents/DrawSpriteComponent.cpp
        Source/Components/DrawComponents/DrawSpriteComponent.h
        Source/Components/DrawComponents/DrawPolygonComponent.cpp
        Source/Components/DrawComponents/DrawPolygonComponent.h
        Source/Actors/MainChar.cpp
        Source/Actors/MainChar.h
        Source/Components/ColliderComponents/AABBColliderComponent.cpp
        Source/Components/ColliderComponents/AABBColliderComponent.h
        Source/Components/DrawComponents/DrawAnimatedComponent.cpp
        Source/Components/DrawComponents/DrawAnimatedComponent.h
        Source/Actors/Block.cpp
        Source/Actors/Block.h
		Source/Actors/Enemies/Junim.cpp
		Source/Actors/Enemies/Junim.h
        Source/Actors/Spawner.cpp
        Source/Actors/Spawner.h
        Source/AudioSystem.cpp
        Source/AudioSystem.h
        Source/UIElements/UIScreen.h
        Source/UIElements/UIScreen.cpp
        Source/UIElements/UIFont.h
        Source/UIElements/UIFont.cpp
        Source/UIElements/UIButton.cpp
        Source/UIElements/UIButton.h
        Source/UIElements/UIText.cpp
        Source/UIElements/UIText.h
        Source/UIElements/UIElement.cpp
        Source/UIElements/UIElement.h
        Source/HUD.cpp
        Source/UIElements/UIImage.cpp
        Source/UIElements/UIImage.h
        Source/SpatialHashing.cpp
        Source/SpatialHashing.h
		Source/Actors/Diamond.h
		Source/Actors/Diamond.cpp
        Source/Actors/Rope.h
        Source/Actors/Rope.cpp
        Source/Actors/Projectile.cpp
        Source/Actors/Projectile.h
        Source/Components/DrawComponents/DrawModularComponent.h
        Source/Components/DrawComponents/DrawModularComponent.cpp
		Source/Actors/Enemies/Gerold.h
		Source/Actors/Enemies/Gerold.cpp
        Source/Actors/Chest.h
        Source/Actors/Chest.cpp
		Source/Actors/CloudEffect.h
        Source/Actors/CloudEffect.cpp
		Source/Actors/Enemies/Enemy.h
		Source/Actors/Enemies/Enemy.cpp
		Source/Actors/FlameEffect.h
        Source/Actors/FlameEffect.cpp
		Source/Actors/Enemies/Iga.h
		Source/Actors/Enemies/Iga.cpp
		Source/Actors/Checkpoint.h
        Source/Actors/Checkpoint.cpp
		Source/Actors/ElementBlock.h
		Source/Actors/ElementBlock.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
