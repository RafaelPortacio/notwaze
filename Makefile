PYTHON ?= python3


.PHONY: run-backend-server
run-backend-server: src/backend/waze-server rj_graph_database.json
	./$<

.PHONY: src/backend/waze-server
src/backend/waze-server:
	$(MAKE) -C src/backend/

rj_graph_database.json: src/extract_data/create_graph_database.py
	$(PYTHON) $<
	sed -i 's/ //g' $@
