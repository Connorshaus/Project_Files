import java.util.Random;
 
public class Knight
{
    private String name;
    int wep;
    int Hitbox;
    Random damage = new Random();
   
   
   
    public Knight()
    {
       
    }
   
    public Knight (String name, int wep, int Hitbox)
    {
        this.name = name;
        this.wep = wep;
        this.Hitbox = Hitbox;
    }
   
    public String getName()
    {
        return this.name;
    }
   
    public void setName(String name)
    {
        this.name = name;
    }
   
    public int getWep()
    {
        return this.wep;
    }
   
    public void setWep(int wep)
    {
        switch(wep)
        {
            case 1:
                this.wep = 1;
                break;
            case 2:
                this.wep = 2;
                break;
            case 3:
                this.wep = 3;
                break;
            case 4:
                this.wep = 4;
                break;
            default:
                System.out.println("invalid weapon");
                break;
           
        }
       
    }
   
    public int getHitbox()
    {
        return this.Hitbox;
    }
   
    public void setHitbox(int Hitbox)
    {
        this.Hitbox = Hitbox;
    }
   
    public String toString()
    {
        return "Knight name: " + this.name + " Weapon: " + this.wep + " Health: " + this.Hitbox;
    }
   
    public int fight()
    {
        switch(this.wep)
        {
           
            case 1:
                //damage between 0-50
                return damage.nextInt(50);
            case 2:    
                //damage between 0-30
                return damage.nextInt(30);
            case 3:    
                //damage between 0-40
                return damage.nextInt(40);
            case 4:    
                //damage between 0-10
                return damage.nextInt(10);
            default:
                return 0;      
           
        }
    }
   
}