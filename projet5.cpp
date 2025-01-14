#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>

// Classe Produit
class Produit {
public:
    int id;
    std::string nom;
    double poids;
    int tempsReception;

    Produit(int id, const std::string& nom, double poids, int tempsReception)
        : id(id), nom(nom), poids(poids), tempsReception(tempsReception) {}
};

// Classe Entrepot
class Entrepot {
public:
    int capaciteMax;
    std::vector<Produit> stock;

    Entrepot(int capacite) : capaciteMax(capacite) {}

    bool ajouterProduit(const Produit& produit) {
        if (stock.size() < capaciteMax) {
            stock.push_back(produit);
            return true;
        }
        std::cout << "Entrepôt plein, impossible d'ajouter le produit." << std::endl;
        return false;
    }

    Produit retirerProduit() {
        if (!stock.empty()) {
            Produit produit = stock.back();
            stock.pop_back();
            return produit;
        }
        return Produit(-1, "Aucun produit", 0, 0);
    }

    void afficherStock() {
        std::cout << "Produits en stock : " << stock.size() << "/" << capaciteMax << std::endl;
    }
};

// Classe QuaiDechargement
class QuaiDechargement {
public:
    int id;
    bool estOccupe;
    int capaciteDechargement; // Capacité en nombre de produits

    QuaiDechargement(int id, int capacite)
        : id(id), estOccupe(false), capaciteDechargement(capacite) {}

    void commencerDechargement() {
        estOccupe = true;
        std::cout << "Quai " << id << " : déchargement commencé.\n";
    }

    void terminerDechargement() {
        estOccupe = false;
        std::cout << "Quai " << id << " : déchargement terminé.\n";
    }
};

// Classe Convoyeur
class Convoyeur {
public:
    int capacite;
    bool estActif;

    Convoyeur(int capacite) : capacite(capacite), estActif(true) {}

    void transporterProduits(int nombreProduits) {
        std::cout << nombreProduits << " produits transportés par le convoyeur.\n";
    }

    void activer() { estActif = true; }
    void desactiver() { estActif = false; }
};

// Classe VehiculeTransport
class VehiculeTransport {
public:
    int id;
    int capaciteMax;
    double vitesseMoyenne; // km/h
    bool estDisponible;
    std::vector<Produit> chargement;

    VehiculeTransport(int id, int capacite, double vitesse)
        : id(id), capaciteMax(capacite), vitesseMoyenne(vitesse), estDisponible(true) {}

    bool chargerProduit(const Produit& produit) {
        if (chargement.size() < capaciteMax) {
            chargement.push_back(produit);
            return true;
        }
        std::cout << "Véhicule " << id << " plein, impossible de charger le produit." << std::endl;
        return false;
    }

    void dechargerProduits() {
        chargement.clear();
        std::cout << "Véhicule " << id << " déchargé.\n";
    }

    void demarrerLivraison(const std::string& destination) {
        estDisponible = false;
        std::cout << "Véhicule " << id << " en livraison vers " << destination << ".\n";
    }

    void terminerLivraison() {
        estDisponible = true;
        std::cout << "Véhicule " << id << " disponible après livraison.\n";
    }
};

// Classe Client
class Client {
public:
    int id;
    std::string nom;
    double positionX;
    double positionY;

    Client(int id, const std::string& nom, double x, double y)
        : id(id), nom(nom), positionX(x), positionY(y) {}
};

// Fonction pour calculer la distance entre deux points géographiques
double calculerDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Classe PlateformeLogistique
class PlateformeLogistique {
private:
    Entrepot entrepot;
    std::vector<Client> clients;
    std::vector<QuaiDechargement> quais;
    std::vector<Convoyeur> convoyeurs;
    std::vector<VehiculeTransport> vehicules;
    int tempsSimule = 0;
    int horizonSimulation;
    int frequenceArrivage;
    int frequenceExpedition;

    int produitsRecus = 0;
    int produitsExpedies = 0;
    double totalTempsEnStock = 0.0;
    double empreinteCarbone = 0.0;

public:
    PlateformeLogistique(int capaciteEntrepot, int horizonSim, int freqArrivage, int freqExpedition)
        : entrepot(capaciteEntrepot), horizonSimulation(horizonSim), frequenceArrivage(freqArrivage), frequenceExpedition(freqExpedition) {}

    void enregistrerClient(int id, const std::string& nom, double x, double y) {
        clients.emplace_back(id, nom, x, y);
    }

    void enregistrerQuai(QuaiDechargement quai) {
        quais.push_back(quai);
    }

    void enregistrerConvoyeur(Convoyeur convoyeur) {
        convoyeurs.push_back(convoyeur);
    }

    void enregistrerVehicule(VehiculeTransport vehicule) {
        vehicules.push_back(vehicule);
    }

    void receptionnerProduits(const Produit& produit) {
        if (entrepot.ajouterProduit(produit)) {
            produitsRecus++;
            std::cout << "Produit " << produit.nom << " reçu.\n";
        }
        entrepot.afficherStock();
    }

    void declencherExpedition() {
        if (!entrepot.stock.empty()) {
            Produit produitExpedie = entrepot.retirerProduit();
            produitsExpedies++;
            totalTempsEnStock += (tempsSimule - produitExpedie.tempsReception);

            // Choix aléatoire d'un client pour la livraison
            int indexClient = rand() % clients.size();
            double distance = calculerDistance(0, 0, clients[indexClient].positionX, clients[indexClient].positionY);
            empreinteCarbone += calculerEmpreinteCarbone(distance);

            std::cout << "Produit " << produitExpedie.nom << " expédié au client " << clients[indexClient].nom << ".\n";
        } else {
            std::cout << "Aucun produit à expédier.\n";
        }
    }

    double calculerEmpreinteCarbone(double distance) {
        const double emissionParKm = 0.25;  // Exemple : 0.25 kg CO2 par km
        return distance * emissionParKm;
    }

    void afficherIndicateurs() {
        std::cout << "\n--- Indicateurs de Performance ---\n";
        std::cout << "Produits reçus : " << produitsRecus << "\n";
        std::cout << "Produits expédiés : " << produitsExpedies << "\n";
        std::cout << "Produits en stock : " << entrepot.stock.size() << "\n";
        std::cout << "Temps moyen de séjour en stock : "
                  << (produitsExpedies > 0 ? totalTempsEnStock / produitsExpedies : 0) << " unités de temps\n";
        std::cout << "Empreinte carbone totale : " << empreinteCarbone << " kg CO2\n";
    }

    void simulerOperation() {
        for (tempsSimule = 0; tempsSimule < horizonSimulation; ++tempsSimule) {
            // Simuler l'arrivage de produits
            if (tempsSimule % frequenceArrivage == 0) {
                receptionnerProduits(Produit(produitsRecus + 1, "Produit X", 2.5, tempsSimule));
            }

            // Simuler une expédition de produits
            if (tempsSimule % frequenceExpedition == 0 && !entrepot.stock.empty()) {
                declencherExpedition();
            }
        }

        afficherIndicateurs();
    }
};

int main() {
    srand(time(0));  // Initialisation pour les nombres aléatoires

    PlateformeLogistique plateforme(100, 100, 10, 15);  // Capacité de l'entrepôt, horizon de simulation, fréquences d'arrivage et d'expédition

    plateforme.enregistrerClient(1, "Client A", 10.0, 15.0);
    plateforme.enregistrerClient(2, "Client B", 20.0, 5.0);

    // Enregistrement des quais, convoyeurs et véhicules de transport
    plateforme.enregistrerQuai(QuaiDechargement(1, 5));
    plateforme.enregistrerConvoyeur(Convoyeur(10));
    plateforme.enregistrerVehicule(VehiculeTransport(1, 50, 80.0));

    // Simulation du fonctionnement de la plateforme
    plateforme.simulerOperation();

    return 0;
}
