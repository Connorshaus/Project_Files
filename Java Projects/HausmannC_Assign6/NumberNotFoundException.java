/*
Hausmann, Connor
COP-3252
Assignment 6
Date: 4/15/2018
*/

// NumberNotFoundException.java
public class NumberNotFoundException extends Exception 
{
   // no-argument constructor specifies default error message
   public NumberNotFoundException() 
   {
      super( "Number not found in array" );
   }
   
   // constructor to allow customized error message
   public NumberNotFoundException( String message )
   {
      super( message );
   }
} // end class NumberNotFoundException