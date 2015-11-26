var wprompt = function(text, func)
{
	if (!w2ui.prompt) {
	        $().w2form({
	            name: 'prompt',
	            style: 'border: 0px; background-color: transparent;',
	            formHTML: 
	                '<div class="w2ui-page page-0">'+
					'	<div style="text-align: center;width:100%;padding-bottom:20; font-weight:bold;">'+text+'</div>'+
	                '   <div style="text-align: center;">'+
	                '       <input name="prompt_inp" type="text" maxlength="100" style="width: 250px"/>'+
	                '    </div>'+
	                '</div>'+
	                '<div class="w2ui-buttons">'+
	                '    <button class="btn" name="reset">Clear</button>'+
	                '    <button class="btn" name="save">Ok</button>'+
	                '</div>',
	            fields: [
	                { field: 'prompt_inp', type: 'text', required: true }
	            ],
	            actions: {
	                "save": function () { 
						if(this.validate().length == 0)
						{
							w2popup.close();
							func(w2ui.prompt.record.prompt_inp);
						}
					},
	                "reset": function () { this.clear(); },
	            }
	        });
	  }
	  // open the popup
	  $().w2popup('open', {
          title   : 'Prompt',
          body    : '<div id="prompt_form" style="width: 100%; height: 100%;"></div>',
          style   : 'padding: 15px 0px 0px 0px',
          showMax : false,
	 	  width: 400,
		  height:200,
          onOpen: function (event) {
              event.onComplete = function () {
                  $('#w2ui-popup #prompt_form').w2render('prompt');
              }
          }
	      });
	//while(spin);
}
