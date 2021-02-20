public class FuelStation {

    private FuelTank nitrogenTank;
    private FuelTank quantumFluidTank;
    private int shipCapacity;
    private int freeSlots;

    FuelStation(int shipCapacity, int nitrogenCapacity, int quantumFluidCapacity){
        this.shipCapacity = shipCapacity;
        this.freeSlots = shipCapacity;
        nitrogenTank = new FuelTank(nitrogenCapacity);
        quantumFluidTank = new FuelTank(quantumFluidCapacity);
    }

    synchronized boolean  requestNitrogen(int nitrogenRequest){

        if(this.nitrogenTank.getFuelVolume() >= nitrogenRequest){
            nitrogenTank.setFuelVolume(nitrogenTank.getFuelVolume()- nitrogenRequest);
            return true;
        }else
            return false;
    }

    synchronized boolean requestQuantumFluid(int quantumFluidRequest){

        if(this.quantumFluidTank.getFuelVolume() >= quantumFluidRequest){
            quantumFluidTank.setFuelVolume(quantumFluidTank.getFuelVolume()- quantumFluidRequest);
            return true;
        }else
            return false;
    }

}
