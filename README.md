#### Test case №1. Correct

| Number        | 1                  |
|-------------|------------------|
| Header        | Accepting donations|
| Precondition  | The charity's website has been launched. The window for entering a donation amount is open. |
|||
| Step  | Expected Result |
|Enter the donation amount. Note: the amount can be selected from a set of fixed values, or entered manually. Minimum amount: 5| When selecting from a set of fixed values, the selected value is displayed in the input field. |
||When entering manually, the entered amount is displayed in the input field. If you enter less than 5, the amount automatically becomes 5.|
|To the right of the donation amount entry field is a clickable drop-down menu for currency selection - from several values EUR, GBP, USD| Currency selection is fixed. No currency conversion. |
|Below the drop-down menu is the percentage of service charge - choose from several values, 5%, 7%, 10%.|When selected, the value is fixed.|
|Click the continue button.|Opens the donor data entry page and the message from the donor.|
|The field your message (Optional). Remaining: 150|As you type, the text of the message is displayed. If you enter more than 150 characters, the input stops.|
|The Fields Your Name (Optional)|When you enter a first name - the text is displayed. If you enter a last name, the text is displayed.|
|The Field with checkbox. Appear as an Anonymous Donor publicly on the fundraising page.|If you select - the first name fields become inactive.|
|The Field Email Address (Required)|When you enter - the email is displayed and fixed.|
|Would you like to be contacted by [...] Selecting an item (Required)|The item selection is fixed.|
|Banking charges: Selecting an item (Required) | The item selection is fixed.|
|Click the back button. |A page opens for selecting a donation amount (the data entered is not saved). |
|Click the continue button.| The payment screen page opens.|
|Entry field Credit or debit card| As you enter - the numbers are fixed and hidden. |
|After entering the correct card number, the pay button becomes active. The button displays the amount including bank charges (+2,5%).| When clicked, payment is made. |

