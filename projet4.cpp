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

    void receptionnerProduits(const Produit& produit) {
        if (entrepot.ajouterProduit(produit)) {
            produitsRecus++;
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
            if (tempsSimule % frequenceExpedition == 0) {
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

    // Simulation du fonctionnement de la plateforme
    plateforme.simulerOperation();

    return 0;
}
