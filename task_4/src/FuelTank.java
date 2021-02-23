public class FuelTank {

    private int capacity;
    private int fuelVolume;

    FuelTank(int c){
        capacity = c;
        fuelVolume = c;
    }

    public int getCapacity() {
        return capacity;
    }

    public int getFuelVolume() {
        return fuelVolume;
    }

    public void setFuelVolume(int fuelVolume) {
        this.fuelVolume = fuelVolume;

        if (0 > this.fuelVolume)
            this.fuelVolume = 0;

    }

    void removeFuel(int amount){
        fuelVolume -= amount;
    }

    public void fillFuel() {
        fuelVolume = capacity;
    }

    public void addFuel(int fuel){
        fuelVolume += fuel;
    }
}
