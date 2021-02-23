public class FuelStation {

    private final FuelTank nitrogenTank;
    private final FuelTank quantumFluidTank;
    private SupplyVehicle nitrogenSupplier;
    private SupplyVehicle quantumFluidSupplier;
    private final int dockingCapacity;
    private int freeSlots;
    private int finishedShips;
    private final int shipTasks;
    private boolean needNitrogen;
    private boolean needQuantumFluid;


    FuelStation(int spaceStationDockingCapacity, int nitrogenCapacity, int quantumFluidCapacity, int tasks) {
        dockingCapacity = spaceStationDockingCapacity;
        freeSlots = spaceStationDockingCapacity;
        nitrogenTank = new FuelTank(nitrogenCapacity);
        quantumFluidTank = new FuelTank(quantumFluidCapacity);
        finishedShips = 0;
        shipTasks = tasks;
        needNitrogen = false;
        needQuantumFluid = false;
    }

    boolean supplyVehicleDock(SupplyVehicle supplyVehicle) {
        return switch (supplyVehicle.getFuelType()) {
            case "nitrogen" -> supplyNitrogen(supplyVehicle);
            case "quantum fluid" -> supplyQuantumFluid(supplyVehicle);
            default -> false;
        };
    }

    synchronized boolean dockRequest() {

        if (0 < freeSlots) {
            freeSlots--;
            sendDockInfo();
            return true;
        } else
            return false;

    }

    synchronized void leaveDock() {
        freeSlots++;
        sendDockInfo();
    }


    synchronized boolean requestNitrogen(int nitrogenRequest) {
        if (this.nitrogenTank.getFuelVolume() >= nitrogenRequest) {
            nitrogenTank.setFuelVolume(nitrogenTank.getFuelVolume() - nitrogenRequest);
            sendNitrogenInfo();
            return true;
        } else {
            requestSupplyOfNitrogen();
            return false;
        }
    }

    synchronized boolean requestQuantumFluid(int quantumFluidRequest) {
        if (this.quantumFluidTank.getFuelVolume() >= quantumFluidRequest) {
            quantumFluidTank.setFuelVolume(quantumFluidTank.getFuelVolume() - quantumFluidRequest);
            sendQuantumFluidInfo();
            return true;
        } else {
            requestSupplyOfQuantumFluid();
            return false;
        }
    }

    private synchronized boolean supplyNitrogen(SupplyVehicle supplyVehicle) {
        int nitrogen = supplyVehicle.getFuelTank().getFuelVolume();

        if (nitrogen <= nitrogenTank.getCapacity() - nitrogenTank.getFuelVolume()) {
            nitrogenTank.addFuel(nitrogen);
            requestNitrogen(supplyVehicle.getFuelRequest());
            supplyVehicle.fueling();
            sendNitrogenInfo();
            return true;

        } else {
            int partVolume = nitrogen - nitrogenTank.getCapacity() - nitrogenTank.getFuelVolume();
            nitrogenTank.fillFuel();
            supplyVehicle.getFuelTank().setFuelVolume(partVolume);
            return false;
        }
    }

    private synchronized boolean supplyQuantumFluid(SupplyVehicle supplyVehicle) {
        int quantumFluid = supplyVehicle.getFuelTank().getFuelVolume();

        if (quantumFluid <= quantumFluidTank.getCapacity() - quantumFluidTank.getFuelVolume()) {
            quantumFluidTank.addFuel(quantumFluid);
            sendQuantumFluidInfo();
            return true;
        } else {
            int partVolume = quantumFluid - quantumFluidTank.getCapacity() - quantumFluidTank.getFuelVolume();
            quantumFluidTank.fillFuel();
            supplyVehicle.getFuelTank().setFuelVolume(partVolume);
            return false;
        }
    }

    private void requestSupplyOfNitrogen() {
        needNitrogen = true;
    }

    private void requestSupplyOfQuantumFluid() {
        needQuantumFluid = true;
    }

    private void sendNitrogenInfo() {
        System.out.println("The nitrogen tank has " + nitrogenTank.getFuelVolume() + " units left");
    }

    private void sendQuantumFluidInfo() {
        System.out.println("The quantum fluid tank has " + quantumFluidTank.getFuelVolume() + " units left");
    }

    private void sendDockInfo() {
        System.out.println("There are " + (dockingCapacity - freeSlots) + " spaceships docked");
        try {
            wait(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void setNitrogenSupplier(SupplyVehicle nitrogenSupplier) {
        this.nitrogenSupplier = nitrogenSupplier;
    }

    public void setQuantumFluidSupplier(SupplyVehicle quantumFluidSupplier) {
        this.quantumFluidSupplier = quantumFluidSupplier;
    }

    synchronized void finishedShip() {
        finishedShips++;

        if (finishedShips >= shipTasks) {
            System.out.println("All ships are finished with their tasks!");
            System.exit(0);
        }
    }

    boolean inNeedOfFuel(String fuelType) {

        if (fuelType.equals("nitrogen") && needNitrogen)
            return true;

        return fuelType.equals("quantum fluid") && needQuantumFluid;
    }
}
