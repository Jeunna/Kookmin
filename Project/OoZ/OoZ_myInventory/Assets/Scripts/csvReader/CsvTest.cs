using UnityEngine;
using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;


public class CsvTest : MonoBehaviour {
	void Start () {

        string testnum_Stick = Convert.ToString(10);
        string testnum_Grass = Convert.ToString(20);
        string testnum_Rose = Convert.ToString(30);

        using (var writer = new CsvFileWriter("Assets/Resources/Inventory.csv"))
		{
			List<string> columns = new List<string>(){"Name", "Item_num"};// making Index Row
			writer.WriteRow(columns);
			columns.Clear();

			columns.Add("Stick"); // Name
			columns.Add(testnum_Stick); // Level
			writer.WriteRow(columns);
			columns.Clear();

			columns.Add("Grass"); // Name
			columns.Add(testnum_Grass); // Level
			writer.WriteRow(columns);
			columns.Clear();

            columns.Add("Rose"); // Name
            columns.Add(testnum_Rose); // Level
            writer.WriteRow(columns);
            columns.Clear();
            
		}
        
    
	}
}
