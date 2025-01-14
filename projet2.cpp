// Modélisation de la plateforme logistique en C++

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

// Classe Produit
class Produit {
public:
    int id;
    std::string nom;
    double poids;

    Produit(int id, const std::string& nom, double poids)
        : id(id), nom(nom), poids(poids) {}
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

    void afficherStock() {
        std::cout << "Produits en stock : " << stock.size() << "/" << capaciteMax << std::endl;
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
    std::vector<QuaiDechargement> quais;
    Entrepot entrepot;
    std::vector<Convoyeur> convoyeurs;
    std::vector<VehiculeTransport> vehicules;
    std::vector<Client> clients;

public:
    PlateformeLogistique(int capaciteEntrepot) : entrepot(capaciteEntrepot) {}

    void ajouterQuai(int capacite) {
        quais.emplace_back(quais.size() + 1, capacite);
    }

    void ajouterConvoyeur(int capacite) {
        convoyeurs.emplace_back(capacite);
    }

    void ajouterVehicule(int capacite, double vitesse) {
        vehicules.emplace_back(vehicules.size() + 1, capacite, vitesse);
    }

    void enregistrerClient(int id, const std::string& nom, double x, double y) {
        clients.emplace_back(id, nom, x, y);
    }

    void receptionnerProduits(const std::vector<Produit>& produits) {
        for (const auto& produit : produits) {
            entrepot.ajouterProduit(produit);
        }
        entrepot.afficherStock();
    }

    void declencherExpedition() {
        std::cout << "Expédition des produits en cours...\n";
        // Logique d'expédition à développer
    }
};

int main() {
    PlateformeLogistique plateforme(100); // Capacité de l'entrepôt : 100 produits

    plateforme.ajouterQuai(10);
    plateforme.ajouterConvoyeur(20);
    plateforme.ajouterVehicule(50, 60.0);
    plateforme.enregistrerClient(1, "Client A", 10.0, 15.0);

    // Simulation de la réception des produits
    std::vector<Produit> produits = { Produit(1, "Produit 1", 2.5), Produit(2, "Produit 2", 1.2) };
    plateforme.receptionnerProduits(produits);

    plateforme.declencherExpedition();

    return 0;
}
