#include "Vehicle.h"

void Vehicle::interactWithPlayer(PlayerSoldier* p){
    p->interactWithVehicle(this);
}

void Vehicle::mount(PlayerSoldier* p) {
    if(driver == nullptr) driver = p;
}

void Vehicle::unmount() {
    driver == nullptr;
}

bool Vehicle::occupied() const {
    return driver != nullptr;
}