#include <vector>
#include <string>
#include <map>
#include <cmath>

// Structure pour représenter une position géographique
struct Location {
    double latitude;
    double longitude;
    
    double calculateDistance(const Location& other) const {
        // Formule de Haversine pour calculer la distance entre deux points
        // ... implémentation ...
        return 0.0; // à compléter
    }
};

// Classe de base pour les produits
class Product {
private:
    std::string reference;
    double weight;
    double volume;
    
public:
    Product(std::string ref, double w, double v) 
        : reference(ref), weight(w), volume(v) {}
    
    std::string getReference() const { return reference; }
    double getWeight() const { return weight; }
    double getVolume() const { return volume; }
};

// Classe pour les quais (déchargement et expédition)
class Dock {
private:
    int id;
    bool isOccupied;
    bool isLoadingDock; // true pour déchargement, false pour expédition
    
public:
    Dock(int dockId, bool loading) 
        : id(dockId), isOccupied(false), isLoadingDock(loading) {}
    
    bool occupy() {
        if (!isOccupied) {
            isOccupied = true;
            return true;
        }
        return false;
    }
    
    void release() { isOccupied = false; }
    bool getIsLoadingDock() const { return isLoadingDock; }
};

// Classe pour le système de manutention
class HandlingSystem {
private:
    std::string type; // ex: "chariot élévateur", "convoyeur"
    double processingCapacity; // capacité de traitement par heure
    bool available;
    
public:
    HandlingSystem(std::string t, double capacity) 
        : type(t), processingCapacity(capacity), available(true) {}
    
    bool isAvailable() const { return available; }
    void setAvailable(bool status) { available = status; }
    double getProcessingCapacity() const { return processingCapacity; }
};

// Classe pour représenter un entrepôt de stockage
class Warehouse {
private:
    double totalCapacity;
    double currentOccupation;
    std::map<std::string, int> inventory; // référence -> quantité
    int numberOfLevels;
    
public:
    Warehouse(double capacity, int levels) 
        : totalCapacity(capacity), currentOccupation(0), numberOfLevels(levels) {}
    
    bool addProduct(const Product& product, int quantity) {
        double newOccupation = currentOccupation + (product.getVolume() * quantity);
        if (newOccupation <= totalCapacity) {
            inventory[product.getReference()] += quantity;
            currentOccupation = newOccupation;
            return true;
        }
        return false;
    }
    
    bool removeProduct(const std::string& reference, int quantity) {
        auto it = inventory.find(reference);
        if (it != inventory.end() && it->second >= quantity) {
            it->second -= quantity;
            // Mettre à jour l'occupation
            return true;
        }
        return false;
    }
};

// Classe pour les véhicules de transport
class TransportVehicle {
private:
    std::string type;
    double capacity;
    bool available;
    Location currentLocation;
    
public:
    TransportVehicle(std::string t, double cap) 
        : type(t), capacity(cap), available(true) {}
    
    bool isAvailable() const { return available; }
    void setAvailable(bool status) { available = status; }
    double getCapacity() const { return capacity; }
};

// Classe principale pour la plateforme logistique
class LogisticsPlatform {
private:
    Location location;
    std::vector<Dock> loadingDocks;
    std::vector<Dock> shippingDocks;
    std::vector<HandlingSystem> handlingSystems;
    std::vector<TransportVehicle> vehicles;
    Warehouse warehouse;
    std::map<std::string, Location> clientLocations;
    
public:
    LogisticsPlatform(Location loc, double warehouseCapacity, int warehouseLevels)
        : location(loc), warehouse(warehouseCapacity, warehouseLevels) {}
    
    void addLoadingDock() {
        loadingDocks.emplace_back(loadingDocks.size(), true);
    }
    
    void addShippingDock() {
        shippingDocks.emplace_back(shippingDocks.size(), false);
    }
    
    void addHandlingSystem(const std::string& type, double capacity) {
        handlingSystems.emplace_back(type, capacity);
    }
    
    void addTransportVehicle(const std::string& type, double capacity) {
        vehicles.emplace_back(type, capacity);
    }
    
    void addClient(const std::string& clientId, const Location& clientLocation) {
        clientLocations[clientId] = clientLocation;
    }
    
    double getDistanceToClient(const std::string& clientId) const {
        auto it = clientLocations.find(clientId);
        if (it != clientLocations.end()) {
            return location.calculateDistance(it->second);
        }
        return -1.0; // Client non trouvé
    }
};