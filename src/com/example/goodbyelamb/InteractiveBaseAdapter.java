package com.example.goodbyelamb;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import android.app.Activity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.TextView;
/**
 * InteractiveBaseAdapter is a adapter that generate 
 * views representing each animal in a list of animals. 
 * @author ragnhild
 *
 */
public class InteractiveBaseAdapter extends BaseAdapter{
	
	//Tag for debugging
	private String tag = "InteractiveArrayAdapter";
	
	private List<Animal> list;
	private final Activity context;
	private boolean showCounted;
	private HashMap<Integer, Animal> allAnimals;

	public InteractiveBaseAdapter(Activity context, List<Animal> list) {
		
		this.context = context;
		this.list = list;
		//Creates a Hashmap with all animals key the animal id and the animal as value
		allAnimals = new HashMap<Integer, Animal>();
		for(int i=0;i<list.size();i++){
			Animal elemnt = list.get(i);
			int animalId = elemnt.getID();
			allAnimals.put(animalId, elemnt);			
		}
		
		showCounted = true;// Show counted is the default option
	}


	static class ViewHolder {
		protected TextView text;
		protected CheckBox checkbox;
	}

	/**
	 * Generates a view for a animal in the list.
	 * The returned view contains a text with the animals ID and a chekbox for indicating
	 * if the animal is counted
	 */
	
	@Override
    public View getView(final int position, View convertView, ViewGroup parent) {
		View view = null;
		
		final ViewHolder viewHolder = new ViewHolder();
        //if (convertView == null) 
        {
                LayoutInflater inflator = context.getLayoutInflater();
                view = inflator.inflate(R.layout.rowlayout, null);
                viewHolder.text = (TextView) view.findViewById(R.id.label);
    			viewHolder.checkbox = (CheckBox) view.findViewById(R.id.check);

    			view.setTag(viewHolder);
    			           
        }
//        else {
//			view = convertView;
//			((ViewHolder) view.getTag()).checkbox.setTag(list.get(position));
//		}
//        
        
        
        viewHolder.checkbox.setTag(list.get(position));
        
        // Update the hashmap with allAnimals if the represented Animals checkbox is changed 
		viewHolder.checkbox
		.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView,
					boolean isChecked) {
				Animal listAnimal = (Animal) viewHolder.checkbox
						.getTag();
				Integer listAnimalId = listAnimal.getID();
				//Update map with all animals
				Animal mapAnimal = allAnimals.get(listAnimalId);
				mapAnimal.setCounted(buttonView.isChecked());
				allAnimals.put(listAnimalId,mapAnimal);
				
				//element.setCounted(buttonView.isChecked());

			}
		});
		
        
		ViewHolder holder = (ViewHolder) view.getTag();
		holder.text.setText(String.valueOf( list.get(position).getID()));
		holder.checkbox.setChecked(list.get(position).getCounted());
        return view;
	}
        @Override
        public int getCount() {
            return list.size();
        }
        @Override
        public Object getItem(int position) {
            return position;
        }
        @Override
        public long getItemId(int position) {
            return position;
        }


	// Method for extract a list from all animals with only uncounted animals
	private ArrayList<Animal> getListUncountedAnimals() {
		ArrayList<Animal> allAnimals = getListAllAnimals();
		Iterator<Animal> it = allAnimals.iterator();
		while(it.hasNext()){
			
			Animal animal = it.next();
			if(animal.getCounted()){
				it.remove();
				Log.w(tag, "removed animal from list with uncounted animals");				
			}
			
		}
		Log.w(tag, "sucseed generate list with uncounted animals");
		Collections.sort(allAnimals);
		return allAnimals;
	}
	
	//Method for extract a list with all animals from hashmap
	private ArrayList<Animal> getListAllAnimals(){
		ArrayList<Animal> allAnimalsList = new ArrayList<Animal>(allAnimals.values());
		Collections.sort(allAnimalsList);
		return allAnimalsList;
	}
	
	/**
	 * Hide all counted animals in the list 
	 */
	
	public void hideAllCounted(){
		list = getListUncountedAnimals();
		showCounted = false;
		notifyDataSetChanged();
	}
	
	/**
	 * Show all animals in the list
	 */
	public void showAll(){
		Log.w(tag,"went in to show all");
		if(showCounted==false){
		list = getListAllAnimals();
		Log.w(tag,"updated list");		
		showCounted = true;
		notifyDataSetChanged();
		}
		
	}



} 