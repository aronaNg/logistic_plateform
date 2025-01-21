#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <numeric>
#include <algorithm>
#include <queue>  // Ajout de la bibliothèque queue

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
    std::queue<Produit> stock;  // Utilisation d'une queue pour le FIFO

    Entrepot(int capacite) : capaciteMax(capacite) {}
    bool estVide() const {
        return stock.empty();
    }

    int tailleStock() const {
        return stock.size();
    }
    bool ajouterProduit(const Produit& produit) {
        if (stock.size() < capaciteMax) {
            stock.push(produit);  // Ajout FIFO
            return true;
        }
        std::cout << "Entrepôt plein, impossible d'ajouter le produit." << std::endl;
        return false;
    }

    Produit retirerProduit() {
        if (!stock.empty()) {
            Produit produit = stock.front();  // On prend le premier produit (FIFO)
            stock.pop();  // On le retire de la queue
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
    double tempsSystemeTotal = 0.0;
    std::vector<double> tempsSystemeProduits;
    
    struct DemandeClient {
        int clientId;
        int quantite;
        int delaiSouhaite;
    };
    std::vector<DemandeClient> demandesEnCours;
    std::map<int, bool> disponibiliteTransporteurs;

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
        disponibiliteTransporteurs[vehicule.id] = true;
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
            // Choix aléatoire d'un client pour la livraison
            int indexClient = rand() % clients.size();
            declencherExpeditionPourClient(clients[indexClient].id, 1);
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

    void ajouterDemande(int clientId, int quantite, int delai) {
        demandesEnCours.push_back({clientId, quantite, delai});
    }

    void mettreAJourDisponibiliteTransporteur(int id, bool disponible) {
        disponibiliteTransporteurs[id] = disponible;
    }

    bool trouverTransporteurDisponible() {
        for (const auto& [id, disponible] : disponibiliteTransporteurs) {
            if (disponible) return true;
        }
        return false;
    }

    double calculerTauxOccupationTransporteurs() {
        int transporteursOccupes = 0;
        for (const auto& [id, disponible] : disponibiliteTransporteurs) {
            if (!disponible) transporteursOccupes++;
        }
        return (vehicules.size() > 0) ? (transporteursOccupes * 100.0) / vehicules.size() : 0;
    }

    void afficherIndicateursEnCours() {
        std::cout << "\n--- Indicateurs en Cours (t=" << tempsSimule << ") ---\n";
        std::cout << "Produits en cours de traitement : " << entrepot.stock.size() << "\n";
        std::cout << "Demandes en attente : " << demandesEnCours.size() << "\n";
        std::cout << "Taux d'occupation des transporteurs : " 
                  << calculerTauxOccupationTransporteurs() << "%\n";
    }

    void traiterDemandesClients() {
        auto it = demandesEnCours.begin();
        while (it != demandesEnCours.end()) {
            if (it->delaiSouhaite >= tempsSimule && trouverTransporteurDisponible()) {
                declencherExpeditionPourClient(it->clientId, it->quantite);
                it = demandesEnCours.erase(it);
            } else {
                ++it;
            }
        }
    }

    void declencherExpeditionPourClient(int clientId, int quantite) {
        auto clientIt = std::find_if(clients.begin(), clients.end(),
            [clientId](const Client& c) { return c.id == clientId; });

        if (clientIt != clients.end() && !entrepot.estVide()) {
            // Chercher un transporteur disponible
            auto transporteurIt = std::find_if(vehicules.begin(), vehicules.end(),
                [](const VehiculeTransport& v) { return v.estDisponible; });

            if (transporteurIt != vehicules.end()) {
                // Marquer le transporteur comme occupé
                transporteurIt->estDisponible = false;
                disponibiliteTransporteurs[transporteurIt->id] = false;

                int produitsLivres = 0;
                while (produitsLivres < quantite && !entrepot.estVide() && 
                    produitsLivres < transporteurIt->capaciteMax) {
                    Produit produitExpedie = entrepot.retirerProduit();  // FIFO : on retire le premier produit
                    produitsExpedies++;
                    double tempsTotal = tempsSimule - produitExpedie.tempsReception;
                    totalTempsEnStock += tempsTotal;
                    tempsSystemeProduits.push_back(tempsTotal);

                    transporteurIt->chargerProduit(produitExpedie);
                    produitsLivres++;
                }

                // Calcul de la distance et empreinte carbone
                double distance = calculerDistance(0, 0, clientIt->positionX, clientIt->positionY);
                empreinteCarbone += calculerEmpreinteCarbone(distance);

                // Simuler la livraison
                transporteurIt->demarrerLivraison(clientIt->nom);
                std::cout << produitsLivres << " produits expédiés au client " 
                        << clientIt->nom << " par le véhicule " << transporteurIt->id << ".\n";

                // Simulation de fin de livraison
                int tempsLivraison = static_cast<int>(distance / transporteurIt->vitesseMoyenne);
                transporteurIt->dechargerProduits();
                transporteurIt->terminerLivraison();
                transporteurIt->estDisponible = true;
                disponibiliteTransporteurs[transporteurIt->id] = true;
            } else {
                std::cout << "Aucun transporteur disponible pour la livraison.\n";
            }
        }
    }


    void afficherIndicateursFinals() {
        afficherIndicateurs();
        
        std::cout << "\n--- Indicateurs Supplémentaires ---\n";
        if (!tempsSystemeProduits.empty()) {
            double tempsMoyenSysteme = std::accumulate(tempsSystemeProduits.begin(), 
                                                     tempsSystemeProduits.end(), 0.0) 
                                     / tempsSystemeProduits.size();
            std::cout << "Temps moyen dans le système : " << tempsMoyenSysteme << " unités\n";
        }
        std::cout << "Demandes non satisfaites : " << demandesEnCours.size() << "\n";
        std::cout << "Taux d'occupation moyen des transporteurs : " 
                  << calculerTauxOccupationTransporteurs() << "%\n";
    }
    void simulerOperation() {
        for (tempsSimule = 0; tempsSimule < horizonSimulation; ++tempsSimule) {
            // Simuler l'arrivage de produits
            if (tempsSimule % frequenceArrivage == 0) {
                receptionnerProduits(Produit(produitsRecus + 1, "Produit X", 2.5, tempsSimule));
            }

            // Traiter les demandes clients
            traiterDemandesClients();

            // Simuler une expédition de produits
            if (tempsSimule % frequenceExpedition == 0 && !entrepot.stock.empty()) {
                declencherExpedition();
            }

            // Afficher les indicateurs tous les 10 pas de temps
            if (tempsSimule % 10 == 0) {
                afficherIndicateursEnCours();
            }
        }

        afficherIndicateursFinals();
    }
};

int main() {
    srand(time(0));  // Initialisation pour les nombres aléatoires

    PlateformeLogistique plateforme(100, 100, 10, 15);  // Capacité de l'entrepôt, horizon de simulation, fréquences d'arrivage et d'expédition

    plateforme.enregistrerClient(1, "Client A", 10.0, 15.0);
    plateforme.enregistrerClient(2, "Client B", 20.0, 5.0);
    plateforme.enregistrerClient(3, "Client C", 15.0, 25.0);
    
    plateforme.ajouterDemande(1, 5, 30);  // Client A veut 5 produits avant t=30
    plateforme.ajouterDemande(2, 3, 50);  // Client B veut 3 produits avant t=50
    plateforme.ajouterDemande(3, 4, 70);  // Client C veut 4 produits avant t=70

    // Enregistrement des quais, convoyeurs et véhicules de transport
    plateforme.enregistrerQuai(QuaiDechargement(1, 5));
    plateforme.enregistrerConvoyeur(Convoyeur(10));
    plateforme.enregistrerVehicule(VehiculeTransport(1, 50, 80.0));
    plateforme.enregistrerVehicule(VehiculeTransport(2, 30, 90.0));

    // Simulation du fonctionnement de la plateforme
    plateforme.simulerOperation();

    return 0;
}
