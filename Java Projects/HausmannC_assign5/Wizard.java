public class Wizard extends Enemy {
    
    @Override
    public String toString() {
        return "Enemy name: WIZARD Attack: ICE / FIRE / MAGIC Health: " + this.health;
    }

    @Override
    public int fight() {
        int type = damage.nextInt(10);
        if(type % 4 == 0) // FIRE ATTACK
            return damage.nextInt(20);
        if(type % 3 == 0) // ICE ATTACK
            return damage.nextInt(10);
        // MAGIC 
        return damage.nextInt(30);
    }
    
}