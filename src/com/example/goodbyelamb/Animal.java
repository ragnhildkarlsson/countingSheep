package com.example.goodbyelamb;

/**
 * Animal represents a animal on the farm
 * @author ragnhild
 *
 */

public class Animal implements Comparable<Animal> {
	private int id;
	private boolean counted;


	public Animal(int id) {
		this.id = id;				
	}


	public int getID(){
		return id;
	}

	/**
	 * Set counted status for the animal. True indicate that the animal has been
	 * counted.
	 * @param counted
	 */
	public void setCounted(Boolean counted){
		this.counted = counted;
	}


	public boolean getCounted(){
		return counted;
	}
	 @Override
	 public String toString(){
		 Integer idString = id;
		 String returnString = idString.toString();
		 return returnString;
	 }
	 
	 /**
	  * Compare to compares animal on ID number.
	  * Make it possible to sort array of animals on ID number
	  */
	 public int compareTo(Animal o){
		 if(this.id==o.getID()){
			 return 0;
		 }
		 if(this.id < o.getID()){
			 return -1;
		 }
		 else {
			 return 1;
		 }
	 }
}
