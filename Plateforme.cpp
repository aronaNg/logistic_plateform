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
#include <iostream>
#include <iomanip>

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

    Entrepot(int capacite) : capaciteMax(capacite) {
        if (capacite <= 0) {
            throw std::invalid_argument("La capacité doit être positive.");
        }
    }
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
        : id(id), estOccupe(false), capaciteDechargement(capacite) {
        if (capacite <= 0) {
            throw std::invalid_argument("La capacité doit être positive.");
        }
    }

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

    Convoyeur(int capacite) : capacite(capacite), estActif(true) {
        if (capacite <= 0) {
            throw std::invalid_argument("La capacité doit être positive.");
        }
    }


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
        : id(id), capaciteMax(capacite), vitesseMoyenne(vitesse), estDisponible(true) {
        if (capacite <= 0 || vitesse <= 0) {
            throw std::invalid_argument("La capacité et la vitesse doivent être positives.");
        }
    }
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
    void log(const std::string& level, const std::string& message, int temps = -1) {
    // Format du temps
    std::string tempsStr = (temps >= 0) ? "[t=" + std::to_string(temps) + "] " : "";

    // Affichage selon niveau
    if (level == "INFO") {
        std::cout << "\033[1;32m[INFO] " << tempsStr << message << "\033[0m\n";  // Vert
    } else if (level == "DEBUG") {
        std::cout << "\033[1;34m[DEBUG] " << tempsStr << message << "\033[0m\n";  // Bleu
    } else if (level == "WARNING") {
        std::cout << "\033[1;33m[WARNING] " << tempsStr << message << "\033[0m\n";  // Jaune
    } else if (level == "ERROR") {
        std::cout << "\033[1;31m[ERROR] " << tempsStr << message << "\033[0m\n";  // Rouge
    } else {
        std::cout << tempsStr << message << "\n";  // Par défaut
    }
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

    void receptionnerProduits(const std::vector<Produit>& produits) {
        int produitsAjoutes = 0;
        for (const auto& produit : produits) {
            if (entrepot.ajouterProduit(produit)) {
                produitsRecus++;
                produitsAjoutes++;
            } else {
                log("WARNING", "Entrepôt plein. Produit " + produit.nom + " non ajouté.", tempsSimule);
            }
        }
        if (produitsAjoutes > 0) {
            log("INFO", std::to_string(produitsAjoutes) + " produits reçus dans l'entrepôt.", tempsSimule);
        }
        entrepot.afficherStock();
    }

    double calculerEmpreinteCarbone(double distance) {
        const double emissionParKm = 0.25;  // Exemple : 0.25 kg CO2 par km
        return distance * emissionParKm;
    }

    void afficherIndicateurs() {
        log("INFO", "--- Indicateurs de Performance ---");
        std::cout << std::setw(30) << "Produits reçus : " << produitsRecus << "\n"
                << std::setw(30) << "Produits expédiés : " << produitsExpedies << "\n"
                << std::setw(30) << "Temps moyen de séjour : "
                << (produitsExpedies > 0 ? totalTempsEnStock / produitsExpedies : 0) << " unités\n"
                << std::setw(30) << "Empreinte carbone totale : " << empreinteCarbone << " kg CO2\n";
    }

    bool verifierClientExiste(int clientId) {
        for (const auto& client : clients) {
            if (client.id == clientId) {
                return true;
            }
        }
        log("WARNING", "Client ID " + std::to_string(clientId) + " introuvable.", tempsSimule);
        return false;
    }

    void ajouterDemande(int clientId, int quantite, int delai) {
        if (verifierClientExiste(clientId)) {
            demandesEnCours.push_back({clientId, quantite, delai});
        }
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

    void declencherExpedition() {
        if (!entrepot.stock.empty()) {
            // Choix aléatoire d'un client pour la livraison
            int indexClient = rand() % clients.size();
            declencherExpeditionPourClient(clients[indexClient].id, 1);
        }
    }


        void traiterDemandesClients() {
            // Trier les demandes par délai souhaité croissant
            std::sort(demandesEnCours.begin(), demandesEnCours.end(),
                    [](const DemandeClient& a, const DemandeClient& b) {
                        return a.delaiSouhaite < b.delaiSouhaite;
                    });
            
            auto it = demandesEnCours.begin();
            while (it != demandesEnCours.end()) {
                if (it->delaiSouhaite >= tempsSimule && trouverTransporteurDisponible()) {
                    if (entrepot.tailleStock() >= it->quantite) {
                        declencherExpeditionPourClient(it->clientId, it->quantite);
                        it = demandesEnCours.erase(it);
                    } else {
                        log("WARNING", "Stock insuffisant pour satisfaire la demande du client " + std::to_string(it->clientId), tempsSimule);
                        ++it;
                    }
                } else {
                    ++it;
                }
            }
        }

        void declencherExpeditionPourClient(int clientId, int quantite) {
            auto clientIt = std::find_if(clients.begin(), clients.end(),
                [clientId](const Client& c) { return c.id == clientId; });

            if (clientIt != clients.end() && !entrepot.estVide()) {
                // Chercher des transporteurs disponibles
                VehiculeTransport* transporteurChoisi = nullptr;

                std::vector<VehiculeTransport*> transporteursDisponibles;
                for (auto& transporteur : vehicules) {
                    if (transporteur.estDisponible) {
                        transporteursDisponibles.push_back(&transporteur);
                    }
                }

                if (!transporteursDisponibles.empty()) {
                    int indexAleatoire = rand() % transporteursDisponibles.size();
                    transporteurChoisi = transporteursDisponibles[indexAleatoire];
                }


                if (transporteurChoisi) {
                    // Marquer le transporteur comme occupé
                    transporteurChoisi->estDisponible = false;
                    disponibiliteTransporteurs[transporteurChoisi->id] = false;
                    log("DEBUG", "Transporteur " + std::to_string(transporteurChoisi->id) + " sélectionné.", tempsSimule);
                    int produitsLivres = 0;
                    while (produitsLivres < quantite && !entrepot.estVide() && 
                        produitsLivres < transporteurChoisi->capaciteMax) {
                        Produit produitExpedie = entrepot.retirerProduit();
                        produitsExpedies++;
                        double tempsTotal = tempsSimule - produitExpedie.tempsReception;
                        totalTempsEnStock += tempsTotal;
                        tempsSystemeProduits.push_back(tempsTotal);

                        transporteurChoisi->chargerProduit(produitExpedie);
                        produitsLivres++;
                    }

                    // Calcul de la distance et empreinte carbone
                    double distance = calculerDistance(0, 0, clientIt->positionX, clientIt->positionY);
                    empreinteCarbone += calculerEmpreinteCarbone(distance);

                    // Simuler la livraison
                    transporteurChoisi->demarrerLivraison(clientIt->nom);
                    log("INFO", std::to_string(produitsLivres) + " produits expédiés au client " + clientIt->nom + ".");

                    // Simulation de fin de livraison
                    int tempsLivraison = static_cast<int>(distance / transporteurChoisi->vitesseMoyenne);
                    transporteurChoisi->dechargerProduits();
                    transporteurChoisi->terminerLivraison();
                    transporteurChoisi->estDisponible = true;
                    disponibiliteTransporteurs[transporteurChoisi->id] = true;
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
                int nombreProduits = 5 + rand() % 15;  
                std::vector<Produit> produitsArrives;
                for (int i = 0; i < nombreProduits; ++i) {
                    produitsArrives.push_back(Produit(produitsRecus + 1, "Produit X", 2.5, tempsSimule));
                }
                receptionnerProduits(produitsArrives);
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

    PlateformeLogistique plateforme(300, 200, 10, 15); 

    plateforme.enregistrerClient(1, "Client A", 10.0, 15.0);
    plateforme.enregistrerClient(2, "Client B", 20.0, 5.0);
    plateforme.enregistrerClient(3, "Client C", 15.0, 25.0);
    plateforme.enregistrerClient(4, "Client D", 10.0, 20.0);
    
    plateforme.ajouterDemande(1, 3, 30);  // Client A veut 5 produits avant t=30
    plateforme.ajouterDemande(2, 4, 80);  
    plateforme.ajouterDemande(3, 5, 90);  
    plateforme.ajouterDemande(4, 6, 100);  

  
    // Enregistrement des quais, convoyeurs et véhicules de transport
    plateforme.enregistrerQuai(QuaiDechargement(1, 5));
    plateforme.enregistrerConvoyeur(Convoyeur(10));
    plateforme.enregistrerVehicule(VehiculeTransport(1, 50, 80.0));
    plateforme.enregistrerVehicule(VehiculeTransport(2, 30, 90.0));

    // Simulation du fonctionnement de la plateforme
    plateforme.simulerOperation();

    return 0;
}
