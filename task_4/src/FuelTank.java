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

    public void setCapacity(int capacity) {
        this.capacity = capacity;
    }

    public int getFuelVolume() {
        return fuelVolume;
    }

    public void setFuelVolume(int fuelVolume) {
        this.fuelVolume = fuelVolume;
    }
}
