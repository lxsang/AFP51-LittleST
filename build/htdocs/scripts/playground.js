var webtty;
var playground_config = {
	layout:{
		name: 'pg_layout',
        panels: [
            { 
            	type: 'main', 
            	style: 'padding: 0px;', 
            	content: 'main',
				toolbar:{
					items:[
						{ type: 'html',  id: 'item6',
		                	html: 
								'<div class="w2ui-field  w2ui-span3">'+
    							'<label>Snippets:</label>'+
    							'<div> <input id="snip_list" onchange="playground_config.select_snippet()"/></div>'+
								'</div>'
		            	},
						{ type: 'spacer' },
						{ type: 'button',  id: 'pl_save',  caption: '', icon: 'fa-save'}
					],
                    onClick: function (event) {
                       	if(event.target = 'pl_save')
						{
							playground_config.save_code();
						}
                    } 
				}
            },
            { 	
            	type: 'right', 
            	size: 300, 
            	resizable:true,
            	style: 'padding: 0px;background-color: #F3DCAF;', 
            	content: '<div id = "pg_log" readonly></div>',
            	toolbar: {
                    items: [
                    	{ type: 'button',  id: 'pl_run',  caption: 'Run', icon: 'fa-caret-right'},
                        { type: 'spacer' },
                        { type: 'button',  id: 'pl_clear_log',  caption: '', icon: 'fa-trash'}
             			//{ type: 'html',  id: 'pl_header',
                		//	html: '<b>Console</b>' 
            			//}
                        
                    ],
                    onClick: function (event) {
                       	if(event.target == 'pl_run')
                       	{
                       		playground_config.run_code();
                       	} 
                       	else if(event.target=='pl_clear_log')
                       	{
                       		$("#pg_log").html("");
                       	}
                    }
                } 
            	
            }
        ]
	},
	open_pl_editor()
	{
		// create popup with editor
		var html = gen_editor_for('pl_editor',
			'Object respondsTo keys do:[:e| e print]');
		//console.log(html);
		w2popup.open({
        	title: 'Play Ground',
        	body:'<div id="pl_editor_main" style="position: absolute; left: 0px; top: 0px; right: 0px; bottom: 0px;"></div>',
        	width:700,
        	height:500,
        	buttons: '',
        	modal:true,
        	opacity: 0,
        	showMax: false,
        	onOpen:function (event) {
        		event.onComplete = function () {
               		$('#w2ui-popup #pl_editor_main').w2render('pg_layout');
                	w2ui.pg_layout.html('main', html);
					$('#snip_list').w2field('list', {
    					url: '/ffvm/scriptbin',
						minLength :0, match: 'contains',
						cacheMax: 2000
            		});
					$(".w2ui-field").css("padding", "0");
					$(".w2ui-field").css("magin", "0");
					$("#snip_list").css("width","250px");
					$("#snip_list").css("margin","0px");
					$(".w2ui-field-helper").css("margin","0");
        		}
			},
			onClose: function(event)
			{
				// close the websocket
				if(webtty) webtty.close();
			},
        	onToggle: function (event) { 
            	event.onComplete = function () {
                w2ui.pg_layout.resize();
            }
        }      
    	}).unlockScreen();
	},
	run_code:function()
	{
		if(pl_editor)
		{
			var code = pl_editor.getValue().trim();
			if(/[a-zA-Z0-9]+/.test(code))
			{
				if(code.charAt(code.length-1) == '.')
					code = code.substring(0,code.length-1);
				playground_config.load_log(code);
			}
			else
				w2alert("Please enter code to execute");
		}
	},
	save_code:function()
	{
		if(pl_editor)
		{
			var code = pl_editor.getValue().trim();
			code = code.replace(/\"/g,"\\\"").replace(/\n/g, "<br>");
			$.post("/ffvm/scriptbin",{code:code})
			.done(function(data){
				if(!data.result)
				{
					w2alert("Cannot save the script:"+ data.msg);
				}
				else
				{
					//$('#snip_list').reLoad();
					//console.log($('#snip_list').w2field());
					$('#snip_list').data('w2field').clearCache()
					$('#snip_list').data('w2field').reset();
					console.log($('#snip_list'));
				}
			});
		}
	},
	select_snippet:function()
	{
		var item = $('#snip_list').data('selected');
		//put the code inside editor
		pl_editor.setValue(item.text.replace(/\<br\>/g, "\n"));
	}
	,
	load_log:function(code)
	{
		//w2alert(code);
		webtty = new EventSource('/ffvm/webtty?code='+code);
		webtty.onmessage = function (e) {
		   $("#pg_log").append(e.data + "<br>");
		   var par = $("#pg_log").parent();
		   par.animate({
			   scrollTop: par.get(0).scrollHeight
		       }, 0);
		   /*var psconsole = $('#pg_log');
		   if(psconsole.length)
		   		psconsole.scrollTop(psconsole[0].scrollHeight - psconsole.height());*/
		};
		webtty.onerror = function(e)
		{
			webtty.close();
		}
	}
}
