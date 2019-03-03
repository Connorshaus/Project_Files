
import java.util.Random;

// This will be your abstract class, so not everything has to be implemented here
// This is the model, thus, all the classes that inherit from this one will
// have to respect this model, but then add some of their own.
public abstract class Enemy {
    
    public static Enemy getRandomEnemy() {
        Random ran = new Random();
        int type = ran.nextInt(3);
        switch(type) {
            case 0: return new Ogre();
            case 1: return new Wizard();
            default: return new Chinchilla();
        }
    }
    
    // This will store the health points of the Enemy, it is protected 
    // because we don't want any class messing with it... except of course
    // its childs. If I make it private then the childs won't be able to see it
    // either.
    int health;
    // We will use this to create the damage, just like you did in Knight
    Random damage = new Random();
    
    // This is the method we want for all the child classes to implement
    // by labeling it "abstract" we force all the childs to "fill it in"
    public abstract int fight();
    
    public void takeDamage(int damage) throws InvalidDamageException {
        // If you get a negative value trhow the exception
        if(damage < 0)
            throw new InvalidDamageException();
        health = getHealth() - damage;
        if (getHealth() < 0)
            health = 0;
    }
    
    public boolean isDead() {
        if (getHealth() == 0)
            return true;
        return false;
    }

    public int getHealth() {
        return health;
    } 
    
    public void setHealth(int health) {
        this.health = health;
    }
    
}
