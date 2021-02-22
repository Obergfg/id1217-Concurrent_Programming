public class FuelStation {

    private final FuelTank nitrogenTank;
    private final FuelTank quantumFluidTank;
    private final SupplyVehicle nitrogenSupplier;
    private final SupplyVehicle quantumFluidSupplier;
    private final int dockingCapacity;
    private int freeSlots;


    FuelStation(int spaceStationDockingCapacity, int nitrogenCapacity, int quantumFluidCapacity) {
        dockingCapacity = spaceStationDockingCapacity;
        freeSlots = spaceStationDockingCapacity;
        nitrogenTank = new FuelTank(nitrogenCapacity);
        quantumFluidTank = new FuelTank(quantumFluidCapacity);
        nitrogenSupplier = new SupplyVehicle(nitrogenCapacity / 2, "nitrogen", this);
        quantumFluidSupplier = new SupplyVehicle(quantumFluidCapacity / 2, "quantum fluid", this);
    }

    void spaceShipDock(SpaceShip spaceShip) {

        sendSpaceStationInfo();
        while (!hasDockingCapacity()) {
            spaceShip.isQueueing();
        }

        spaceShip.hasDocked();

        switch (spaceShip.getRequestState()) {
            case "nitrogen":
                if (spaceShipRequestNitrogen(spaceShip))
                    spaceShip.flying(false);
                else {
                    spaceShip.flying(true);
                    spaceShipDock(spaceShip);
                }
                break;
            case "quantum fluid":
                if (spaceShipRequestQuantumFluid(spaceShip))
                    spaceShip.flying(false);
                else {
                    spaceShip.flying(true);
                    spaceShipDock(spaceShip);
                }
                break;
            case "both fuels":
                if (spaceShipRequestNitrogen(spaceShip)) {
                    if (spaceShipRequestQuantumFluid(spaceShip))
                        spaceShip.flying(false);
                    else {
                        spaceShip.setQuantumState();
                        spaceShip.flying(true);
                        spaceShipDock(spaceShip);
                    }
                } else {
                    spaceShip.flying(true);
                    spaceShipDock(spaceShip);
                }
        }

        increaseDockingCapacity();
        sendSpaceStationInfo();
    }

    void supplyVehicleDock(SupplyVehicle supplyVehicle){

        while (!hasDockingCapacity()){
            supplyVehicle.isQueueing();
        }

        supplyVehicle.hasDocked();

        switch (supplyVehicle.getFuelType()) {
            case "nitrogen" -> supplyNitrogen(supplyVehicle);
            case "quantum fluid" -> supplyQuantumFluid(supplyVehicle);
        }

        increaseDockingCapacity();
        sendSpaceStationInfo();
    }

    private synchronized boolean hasDockingCapacity() {

        if (0 < freeSlots) {
            freeSlots--;
            return true;
        } else
            return false;

    }

    private synchronized void increaseDockingCapacity() {
        freeSlots++;
    }


    private synchronized boolean spaceShipRequestNitrogen(SpaceShip spaceShip) {

        int nitrogenRequest = spaceShip.getNitrogen().getCapacity() - spaceShip.getNitrogen().getFuelVolume();

        if (this.nitrogenTank.getFuelVolume() >= nitrogenRequest) {
            nitrogenTank.setFuelVolume(nitrogenTank.getFuelVolume() - nitrogenRequest);
            spaceShip.fillNitrogen();
            return true;
        } else {
            nitrogenSupplier.requestFuelSupply();
            return false;
        }
    }

    private synchronized boolean spaceShipRequestQuantumFluid(SpaceShip spaceShip) {

        int quantumFluidRequest = spaceShip.getQuantumFluid().getCapacity() - spaceShip.getQuantumFluid().getFuelVolume();

        if (this.quantumFluidTank.getFuelVolume() >= quantumFluidRequest) {
            quantumFluidTank.setFuelVolume(quantumFluidTank.getFuelVolume() - quantumFluidRequest);
            spaceShip.fillQuantumFluid();
            return true;
        } else {
            quantumFluidSupplier.requestFuelSupply();
            return false;
        }
    }

    private synchronized void supplyNitrogen(SupplyVehicle supplyVehicle){

    }

    private synchronized void supplyQuantumFluid(SupplyVehicle supplyVehicle){



    }


    private void sendSpaceStationInfo() {
        System.out.println("There are " + (dockingCapacity - freeSlots) + " spaceships docked\n" +
                "The nitrogen tank has " + nitrogenTank.getFuelVolume() + " units left\n" +
                "The quantum fluid tank has " + quantumFluidTank.getFuelVolume() + " units left");
    }
}
