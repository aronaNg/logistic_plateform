#include <iostream>
#include <iomanip>
#include <limits>
#include "Plateforme.cpp"
class SimulationInterface {
private:
    PlateformeLogistique& plateforme;

    void afficherMenuPrincipal() {
        std::cout << "\n--- Menu Simulation Logistique ---\n";
        std::cout << "1. Configurer Simulation\n";
        std::cout << "2. Ajouter Clients\n";
        std::cout << "3. Ajouter Demandes Clients\n";
        std::cout << "4. Configurer Infrastructure\n";
        std::cout << "5. Lancer Simulation\n";
        std::cout << "6. Afficher Indicateurs Courants\n";
        std::cout << "7. Quitter\n";
        std::cout << "Votre choix : ";
    }

    void configurationSimulation() {
        int capacite, horizon, freqArrivage, freqExpedition;
        std::cout << "Capacité Entrepôt : ";
        std::cin >> capacite;
        std::cout << "Horizon Simulation : ";
        std::cin >> horizon;
        std::cout << "Fréquence Arrivage Produits : ";
        std::cin >> freqArrivage;
        std::cout << "Fréquence Expédition : ";
        std::cin >> freqExpedition;

        plateforme = PlateformeLogistique(capacite, horizon, freqArrivage, freqExpedition);
        std::cout << "Configuration enregistrée.\n";
    }

    int lireEntier(const std::string& message) {
        int valeur;
        while (true) {
            std::cout << message;
            if (std::cin >> valeur) break;
            std::cout << "Entrée invalide. Veuillez entrer un nombre entier.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return valeur;
    }


    void ajouterClients() {
        char continuer;
        do {
            int id = lireEntier("ID Client : ");
            std::cout << "Nom Client : ";
            std::string nom;
            std::cin >> nom;
            int x = lireEntier("Position X : ");
            int y = lireEntier("Position Y : ");

            if (x >= 0 && y >= 0) {
                plateforme.enregistrerClient(id, nom, x, y);
                std::cout << "Client ajouté avec succès.\n";
            } else {
                std::cout << "Position invalide. Les coordonnées doivent être positives.\n";
            }

            std::cout << "Ajouter un autre client ? (o/n) : ";
            std::cin >> continuer;
        } while (continuer == 'o' || continuer == 'O');
    }

    void ajouterDemandesClients() {
        int clientId, quantite, delai;
        char continuer;

        do {
            std::cout << "ID Client : ";
            std::cin >> clientId;
            std::cout << "Quantité Produits : ";
            std::cin >> quantite;
            std::cout << "Délai Souhaité : ";
            std::cin >> delai;

            plateforme.ajouterDemande(clientId, quantite, delai);

            std::cout << "Ajouter une autre demande ? (o/n) : ";
            std::cin >> continuer;
        } while (continuer == 'o' || continuer == 'O');
    }

    void configurerInfrastructure() {
        int choix;
        std::cout << "\n--- Configuration Infrastructure ---\n";
        std::cout << "1. Ajouter Quai de Déchargement\n";
        std::cout << "2. Ajouter Convoyeur\n";
        std::cout << "3. Ajouter Véhicule Transport\n";
        std::cout << "Votre choix : ";
        std::cin >> choix;

        switch(choix) {
            case 1: {
                int id, capacite;
                std::cout << "ID Quai : ";
                std::cin >> id;
                std::cout << "Capacité Déchargement : ";
                std::cin >> capacite;
                plateforme.enregistrerQuai(QuaiDechargement(id, capacite));
                break;
            }
            case 2: {
                int capacite;
                std::cout << "Capacité Convoyeur : ";
                std::cin >> capacite;
                plateforme.enregistrerConvoyeur(Convoyeur(capacite));
                break;
            }
            case 3: {
                int id, capacite;
                double vitesse;
                std::cout << "ID Véhicule : ";
                std::cin >> id;
                std::cout << "Capacité Max : ";
                std::cin >> capacite;
                std::cout << "Vitesse Moyenne : ";
                std::cin >> vitesse;
                plateforme.enregistrerVehicule(VehiculeTransport(id, capacite, vitesse));
                break;
            }
            default:
                std::cout << "Choix invalide.\n";
        }
    }

public:
    SimulationInterface(PlateformeLogistique& platform) : plateforme(platform) {}

    void executerInterface() {
        int choix;
        do {
            afficherMenuPrincipal();
            std::cin >> choix;

            switch(choix) {
                case 1: configurationSimulation(); break;
                case 2: ajouterClients(); break;
                case 3: ajouterDemandesClients(); break;
                case 4: configurerInfrastructure(); break;
                case 5: plateforme.simulerOperation(); break;
                case 6: plateforme.afficherIndicateursEnCours(); break;
                case 7: std::cout << "Fin de simulation.\n"; break;
                default: std::cout << "Choix invalide.\n";
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        } while (choix != 7);
    }
};

int main() {
    srand(time(0));
    PlateformeLogistique plateforme(100, 100, 10, 15);

    plateforme.enregistrerClient(1, "NGOM", 10.0, 15.0);
    plateforme.enregistrerClient(2, "HOFFMANN", 20.0, 5.0);
    plateforme.enregistrerClient(3, "Testeur", 15.0, 25.0);
    
    plateforme.ajouterDemande(1, 1, 30);
    plateforme.ajouterDemande(2, 23, 70);

    plateforme.enregistrerQuai(QuaiDechargement(1, 5));
    plateforme.enregistrerConvoyeur(Convoyeur(10));
    plateforme.enregistrerVehicule(VehiculeTransport(1, 50, 80.0));
    plateforme.enregistrerVehicule(VehiculeTransport(2, 30, 90.0));

    SimulationInterface interface(plateforme);
    interface.executerInterface();
    return 0;
}