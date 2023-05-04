#include "level_01.h"

Level_01::Level_01()
{
}
Level_01::~Level_01()
{
}

double Level_01::GetLastEnemySpawnedTime()
{
    return randomEnemySpawnTimer;
}

void Level_01::SetLastEnemySpawnedTime(double lastTime)
{
    randomEnemySpawnTimer = lastTime;
}

void Level_01::RandomEnemySpawn()
{
    double timeNow = GetTime();

    if (randomEnemySpawnTimer + 10.00 <= timeNow)
    {
        if (!randomEnemyTextureLoaded)
        {
            InitRandomEnemyTexture();
            randomEnemyTextureLoaded = true;
        }
        if (!randomEnemyBulletTextureLoaded)
        {
            InitRandomEnemyBulletTexture();
        }

        SetLastEnemySpawnedTime(GetTime());
        Enemy randomEnemy;
        ResetRandomEnemy(randomEnemy);

        randomEnemies.push_back(randomEnemy);
    }
}

void Level_01::UpdateRandomEnemies()
{
    if (randomEnemySpawnTimer == 0)
        return;
    for (int i = 0; i < randomEnemies.size(); i++)
    {
        if (!randomEnemies[i].active)
        {
            return;
        }

        if (randomEnemies[i].position.x >= GetScreenWidth() || randomEnemies[i].position.y >= GetScreenHeight())
        {
            randomEnemies[i].active = false;
        }
        else
        {
            randomEnemies[i].position.x += (randomEnemies[i].speed * 1.5) * GetFrameTime();
            randomEnemies[i].position.y += randomEnemies[i].speed * GetFrameTime();
        }
    }
}

void Level_01::DrawRandomEnemies(Player &player)
{
    if (randomEnemySpawnTimer == 0)
    {
        return;
    }

    float frameWidth = (float)(randomEnemyTexture.width / 3); // Sprite one frame rectangle width
    float frameHeight = (float)(randomEnemyTexture.height);   // Sprite one frame rectangle height

    for (int i = 0; i < randomEnemies.size(); i++)
    {
        if (!randomEnemies[i].active)
        {
            randomEnemies.erase(randomEnemies.begin() + i);
            return;
        }

        randomEnemies[i].frameCounter++;
        if (randomEnemies[i].frameCounter >= 5)
        {
            randomEnemies[i].frameCounter = 0;
            randomEnemies[i].currentFrame++;
        }
        Rectangle frameRec = {randomEnemies[i].position.x, randomEnemies[i].position.y, frameWidth, frameHeight};
        frameRec.x = frameWidth * randomEnemies[i].currentFrame;
        frameRec.y = frameHeight;
        randomEnemies[i].UpdateEnemyAttack(randomEnemies[i].position.x, randomEnemies[i].position.y, randomEnemyBulletTexture, 50, 11, 3);
        DrawTextureRec(randomEnemies[i].enemyTexture, frameRec, {randomEnemies[i].position.x, randomEnemies[i].position.y}, WHITE);

        // Enemy killed and removed, explosion
        if (randomEnemies[i].health <= 0 && !randomEnemies[i].active)
        {
            randomEnemies[i].EnemyExplosion(CalculateObjectSizeY(200.f), CalculateObjectSizeY(8.f));
        }

        // Checking if enemy is hit
        if (randomEnemies[i].health > 0 && randomEnemies[i].active)
        {
            randomEnemies[i].isHit(player.leftBullets, player.rightBullets, player.score);
        }
    }
}

void Level_01::InitRandomEnemyBulletTexture()
{
    Image randomBulletImg = LoadImage("./media/randomSpawnBullet.png");
    randomEnemyBulletTexture = LoadTextureFromImage(randomBulletImg);
    randomEnemyBulletTextureLoaded = true;
    UnloadImage(randomBulletImg);
}

void Level_01::InitRandomEnemyTexture()
{
    Image randomEnemyImg = LoadImage("./media/randomEnemy1.png");
    randomEnemyTexture = LoadTextureFromImage(randomEnemyImg);
    UnloadImage(randomEnemyImg);
}

void Level_01::ResetRandomEnemy(Enemy &randomEnemy)
{
    randomEnemy.health = 200;
    randomEnemy.active = true;
    randomEnemy.enemyTexture = randomEnemyTexture;
    randomEnemy.position = {100, 100};
    randomEnemy.speed = 100;
    randomEnemy.scoreValue = 250;
    randomEnemy.frameCounter = 0;
    randomEnemy.currentFrame = 0;
    randomEnemy.active = true;
}

// DEFAULT ENEMY
void Level_01::InitDefaultEnemyBulletTexture()
{
    Image defaultEnemyBulletImg = LoadImage("./media/bullet_enemy_0.png");
    ImageResize(&defaultEnemyBulletImg, CalculateObjectSizeX(defaultEnemyBulletImg.width), CalculateObjectSizeY(defaultEnemyBulletImg.height));
    Texture2D loadedDefaultenemyBulletTexture = LoadTextureFromImage(defaultEnemyBulletImg);
    UnloadImage(defaultEnemyBulletImg);
    defaultEnemyBulletTexture = loadedDefaultenemyBulletTexture;
}

void Level_01::DrawMultipleEnemies(std::vector<int> &xPositions, Player &player)
{
    if (defaultEnemies.size() <= 0)
    {
        defaultEnemyTexture = LoadDefaultEnemyTexture();
        InitDefaultEnemyBulletTexture();
    }
    for (int i = 0; i < 4; i++)
    {
        // First spawn,initiating 4 enemies
        if (defaultEnemies.size() < 4)
        {
            Enemy defEnemy;
            defEnemy.InitDefaultEnemyDefaults(xPositions[i], defaultEnemyTexture);
            defEnemy.ResetDefaultEnenmy();
            defaultEnemies.push_back(defEnemy);
        }

        // Activate new enemy
        if (!defaultEnemies[i].active && defaultEnemies[i].health >= 100)
            defaultEnemies[i].active = true;

        // Enemy killed and removed, explosion
        if (defaultEnemies[i].health <= 0 && !defaultEnemies[i].active)
        {
            defaultEnemies[i].EnemyExplosion(CalculateObjectSizeY(200.f), CalculateObjectSizeY(8.f));
        }

        // Hover and draw default enenmy movements
        if (defaultEnemies[i].active && defaultEnemies[i].health > 0)
        {
            if (defaultEnemies[i].position.y <= 150)
                defaultEnemies[i].position.y += defaultEnemies[i].speed;
            else
                defaultEnemies[i].hover(CalculateByPixelsX(xPositions[i]));

            DrawTexture(defaultEnemies[i].enemyTexture, defaultEnemies[i].position.x, defaultEnemies[i].position.y, WHITE);
        }

        // Update enemy attack (draw and update bullet status)
        defaultEnemies[i].UpdateEnemyDefaultAttack(defaultEnemies[i].position.x, defaultEnemyBulletTexture);

        // Checking if enemies is hit
        if (defaultEnemies[i].health > 0 && defaultEnemies[i].active)
        {
            defaultEnemies[i].isHit(player.leftBullets, player.rightBullets, player.score);
        }
    }
}

void Level_01::UnloadMultipleEnemies()
{
    for (int i = 0; i < defaultEnemies.size(); i++)
    {

        defaultEnemies[i].UnloadEnemy();
        defaultEnemies[i].ResetDefaultEnenmy();
        defaultEnemies[i].enemyBullets.clear();
    }
    defaultEnemies.clear();
}
//

// BOSS
void Level_01::SpawnBoss()
{
    boss.InitBoss(CalculateXCoord(100 / 2), 500, 2);
}

void Level_01::UpdateBoss()
{
}

void Level_01::isBossHit()
{
}
//
void Level_01::LevelComplete()
{
}

void Level_01::UnloadLevel1()
{
    UnloadTexture(randomEnemyTexture);
    UnloadTexture(defaultEnemyBulletTexture);
    UnloadTexture(defaultEnemyTexture);
    randomEnemies.clear();
    defaultEnemies.clear();
}
