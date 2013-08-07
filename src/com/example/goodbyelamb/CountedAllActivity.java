package com.example.goodbyelamb;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.View;

public class CountedAllActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_counted_all);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.counted_all, menu);
		return true;
	}
	/** Called when the user clicks the CountAgainButton
	 *  Starts the StartScreenActivity  
	 **/
	public void countAgain(View view) {
		Intent intent = new Intent(this, StartScreenActivity.class);
		startActivity(intent);
	}

}
