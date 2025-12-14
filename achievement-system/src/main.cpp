/**
 * @file main.cpp
 * @brief Point d'entrée du Achievement System Designer
 *
 * Ce fichier contient uniquement la logique d'orchestration :
 * - Initialisation des systèmes
 * - Boucle principale
 * - Rendu ImGui
 * - Simulation des événements
 * - Fermeture propre de l'application
 *
 * Aucune logique métier ne doit être définie ici.
 */

#include "AchievementManager.h"
#include "EventSystem.h"
#include "UIManager.h"

#include <chrono>

/**
 * @brief Point d'entrée du programme
 *
 * @return int Code de sortie
 */
int main()
{
    /* ================= INITIALISATION ================= */

    UIManager uiManager;                                        //creation des objets principaux(separation des responsabilites)
    AchievementManager achievementManager;
    EventSystem eventSystem;

    uiManager.Initialize();     

    achievementManager.LoadFromFile("assets/achievements.json");               //chargement des donnees depuis un fichier json 

    /* Horloge pour maintenir ~60 FPS */
    const std::chrono::milliseconds FRAME_DURATION(16);

    /* ================= BOUCLE PRINCIPALE ================= */

    while (uiManager.IsRunning()) {

        auto frameStart = std::chrono::high_resolution_clock::now();

        /* --- 1. Réception / simulation des événements --- */
        eventSystem.PollEvents();

        /* --- 2. Traitement des achievements --- */
        achievementManager.ProcessEvents(eventSystem);

        /* --- 3. Interface graphique --- */
        uiManager.BeginFrame();

        uiManager.RenderAchievements(achievementManager);
        uiManager.RenderEventSimulator(eventSystem);

        uiManager.EndFrame();

        /* --- 4. Limitation FPS (≈ 60 FPS) --- */
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (elapsed < FRAME_DURATION) {
            std::this_thread::sleep_for(FRAME_DURATION - elapsed);
        }
    }

    /* ================= FERMETURE PROPRE ================= */

    uiManager.Shutdown();

    return 0;
}